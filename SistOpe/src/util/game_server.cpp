#include "Game.h"
#include "Menu.h"
#include <sstream>
#include "config/EnvConfig.h"
#include <algorithm>
#include <cerrno>
#include <csignal>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <random>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

// esto para poder conservar los logs, un cierre ordenado
volatile sig_atomic_t shutdownRequested = 0;
void requestShutdown(int) { shutdownRequested = 1; }

string getTeamName(int team) {
    if (team < 0) return "-";
    return "Equipo " + to_string(team + 1);
}

// registro y formato de logs
void logEvent(GameState& game, const string& player, int team,
               const string& action, const string& details) {
    string logDetails = details;
    for (char& character : logDetails)
        if (character == '\n' || character == '\r') character = ' ';
    time_t now = time(nullptr);
    tm timestamp{};
    localtime_r(&now, &timestamp);
    game.logFile << put_time(&timestamp, "%Y-%m-%d %H:%M:%S") << ';' << player << ';'
                << getTeamName(team) << ';' << action << ';' << logDetails << '\n';
    game.logFile.flush();
    if (!game.logFile) throw runtime_error("No se pudo escribir el log del juego.");
}
// cuantos jugadores tiene conectado un equipo
int countTeamPlayers(const GameState& game, int team) {
    int count = 0;
    for (const GamePlayer& player : game.players)
        if (player.socket >= 0 && player.team == team) ++count;
    return count;
}
// escoge minequipos -> completa -> agrega equipos hasta maxequipos -> luego rellena hasta no haber cupo
int chooseTeam(const GameState& game) {
    int activeTeams = 0;
    int emptyTeam = -1;
    int smallestTeam = -1;
    int smallestCount = game.maxPerTeam + 1;
    for (int team = 0; team < game.maxTeams; ++team) {
        int count = countTeamPlayers(game, team);
        if (count == 0) {
            if (emptyTeam == -1) emptyTeam = team;
        } else {
            ++activeTeams;
            if (count < smallestCount) {
                smallestTeam = team;
                smallestCount = count;
            }
        }
    }
    if (activeTeams < game.minTeams) return emptyTeam;
    if (smallestCount < game.minPerTeam) return smallestTeam;
    if (activeTeams < game.maxTeams) return emptyTeam;
    if (smallestCount < game.maxPerTeam) return smallestTeam;
    return -1;
}

bool canStartGame(const GameState& game) {
    int activeTeams = 0;
    for (int team = 0; team < game.maxTeams; ++team) {
        int count = countTeamPlayers(game, team);
        if (count == 0) continue;
        if (count < game.minPerTeam || count > game.maxPerTeam) return false;
        ++activeTeams;
    }
    return activeTeams >= game.minTeams;
}
// mensaje del estadoprevio al juego
string getGameStatus(const GameState& game) {
    ostringstream message;
    message << endl << "----------------------------------------" << endl
            << "ESTADO DE LA PARTIDA" << endl
            << "----------------------------------------" << endl;
    int activeTeams = 0;
    for (int team = 0; team < game.maxTeams; ++team) {
        int count = countTeamPlayers(game, team);
        if (count == 0 && game.positions[team] == 0) continue;
        if (count > 0) ++activeTeams;
        message << getTeamName(team) << " | Posicion: " << game.positions[team]
                << " | Jugadores: " << count << endl;
    }
    message << "Equipos activos: " << activeTeams << endl
            << "Meta: " << game.goal << endl;
    if (game.started) {
        message << "En juego | Turno del jugador ID: " << game.currentTurn << endl;
    } else {
        message << endl << "ESPERANDO JUGADORES" << endl
                << "Equipos permitidos: " << game.minTeams << " a " << game.maxTeams << endl
                << "Jugadores por equipo: " << game.minPerTeam << " a " << game.maxPerTeam << endl
                << "Para comenzar: /iniciar" << endl;
    }
    message << "----------------------------------------";
    return message.str();
}
// mensaje a un solo jugador
void sendMessage(GamePlayer& player, const string& message) {
    if (!sendAll(player.socket, message + "\n")) player.sendFailed = true;
}
// mensaje a todos los jugadores
void broadcastMessage(GameState& game, const string& message) {
    cout << message << endl;
    for (GamePlayer& player : game.players)
        if (player.socket >= 0 && !player.sendFailed) sendMessage(player, message);
}
// avanza turno en orden de conexion
void advanceTurn(GameState& game, int previousId) {
    GamePlayer* nextPlayer = nullptr;
    for (GamePlayer& player : game.players) {
        if (player.socket < 0 || player.team < 0) continue;
        if (nextPlayer == nullptr) nextPlayer = &player;
        if (player.id > previousId) { nextPlayer = &player; break; }
    }
    if (nextPlayer == nullptr) return;
    game.currentTurn = nextPlayer->id;
    logEvent(game, nextPlayer->name, nextPlayer->team, "TURNO", to_string(game.currentTurn));
    broadcastMessage(game, "TURNO " + nextPlayer->name + " ID=" +
        to_string(game.currentTurn) + " equipo=" + getTeamName(nextPlayer->team) + ". Usa /lanzar.");
}
// marca partida como terminada, registra el resultado y anuncia el resultado
void finishGame(GameState& game, int winner, const string& reason) {
    if (game.finished) return;
    game.finished = true;
    string result = winner < 0 ? "Sin ganador" : "Gana " + getTeamName(winner);
    logEvent(game, "SERVIDOR", winner, "RESULTADO_FINAL", result);
    logEvent(game, "SERVIDOR", winner, "FIN_PARTIDA", reason);
    broadcastMessage(game, "FIN " + result + ". " + reason);
}
// maneja la desconcexion, cierra el socket -> registra la desconexion -> evalua como continuar o terminar el juego
void disconnectPlayer(GameState& game, GamePlayer& player, const string& reason) {
    if (player.socket < 0) return;
    close(player.socket);
    player.socket = -1;
    logEvent(game, player.name.empty() ? "ID=" + to_string(player.id) : player.name,
              player.team, "DESCONEXION", reason);
    if (player.team < 0 || game.finished) return;
    broadcastMessage(game, "SALE " + player.name + ". " + reason);
    if (!game.started) { broadcastMessage(game, getGameStatus(game)); return; }
    if (countTeamPlayers(game, player.team) == 0) {
        logEvent(game, player.name, player.team, "EQUIPO_ELIMINADO", "Sin jugadores conectados");
        broadcastMessage(game, getTeamName(player.team) + " quedo sin jugadores.");
    }
    int activeTeams = 0;
    int remainingTeam = -1;
    for (int team = 0; team < game.maxTeams; ++team) {
        if (countTeamPlayers(game, team) > 0) {
            ++activeTeams;
            remainingTeam = team;
        }
    }
    if (activeTeams == 0) finishGame(game, -1, "No quedan equipos con jugadores.");
    else if (activeTeams == 1) finishGame(game, remainingTeam, "Solo queda un equipo con jugadores.");
    else if (game.currentTurn == player.id) advanceTurn(game, player.id);
}
// para identificar jugadores, luego asigna a aun equipo
void registerPlayer(GameState& game, GamePlayer& player, const string& name) {
    if (name.empty() || name.size() > 20) {
        sendMessage(player, "ERROR Nombre: usa 1 a 20 letras, numeros o guion bajo."); return;
    }
    for (char character : name) {
        bool valid = (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') ||
                      (character >= '0' && character <= '9') || character == '_';
        if (!valid) { sendMessage(player, "ERROR Nombre: usa letras, numeros o guion bajo."); return; }
    }
    for (const GamePlayer& other : game.players)
        if (other.socket >= 0 && other.name == name) {
            sendMessage(player, "ERROR Nombre ocupado. Ingresa otro nombre."); return;
        }
    if (game.started) { sendMessage(player, "ERROR La partida ya comenzo."); return; }
    int team = chooseTeam(game);
    if (team < 0) {
        sendMessage(player, "ERROR Equipos completos."); return;
    }
    player.name = name;
    player.team = team;
    logEvent(game, name, team, "IDENTIFICACION", to_string(player.id));
    logEvent(game, name, team, "INCORPORACION_EQUIPO", getTeamName(team));
    sendMessage(player, "BIENVENIDO " + name + " ID=" + to_string(player.id) +
           " equipo=" + getTeamName(team));
    broadcastMessage(game, "ENTRA " + name + " al " + getTeamName(team));
    broadcastMessage(game, getGameStatus(game));
}
// interpreta lo que ingresa el usuario y hace los logs
void processCommand(GameState& game, GamePlayer& player, const string& command) {
    if (command == "/salir") { disconnectPlayer(game, player, "Salida voluntaria"); return; }
    if (player.team < 0) { registerPlayer(game, player, command); return; }
    if (command == "/estado") { sendMessage(player, getGameStatus(game)); return; }
    if (command == "/ayuda") {
        sendMessage(player, "COMANDOS /iniciar /lanzar /estado /ayuda /salir"); return;
    }
    if (command == "/iniciar" && !game.started) {
        if (!canStartGame(game)) {
            sendMessage(player, "ERROR No se cumple la condicion de inicio. " + getGameStatus(game)); return;
        }
        game.started = true;
        logEvent(game, player.name, player.team, "INICIO_PARTIDA", getGameStatus(game));
        broadcastMessage(game, "INICIO Carrera por equipos.");
        advanceTurn(game, 0);
        return;
    }
    if (command != "/lanzar" || !game.started || game.currentTurn != player.id) {
        sendMessage(player, "ERROR Accion no permitida en este estado o fuera de tu turno.");
        logEvent(game, player.name, player.team, "ACCION_RECHAZADA", "Comando invalido o fuera de turno");
        return;
    }
    logEvent(game, player.name, player.team, "ACCION", "LANZAR");
    int roll = uniform_int_distribution<int>(1, game.diceFaces)(game.randomEngine);
    logEvent(game, player.name, player.team, "RESULTADO_ACCION", to_string(roll));
    long long previousPosition = game.positions[player.team];
    game.positions[player.team] += roll;
    logEvent(game, player.name, player.team, "AVANCE", to_string(previousPosition) +
              " -> " + to_string(game.positions[player.team]));
    broadcastMessage(game, "DADO " + player.name + " obtuvo " + to_string(roll) +
        ". " + getTeamName(player.team) + " avanza a " + to_string(game.positions[player.team]));
    if (game.positions[player.team] >= game.goal) finishGame(game, player.team, "Meta alcanzada.");
    else advanceTurn(game, player.id);
}
// recibe datos de un jugador y detecta desconexion
void receiveCommands(GameState& game, GamePlayer& player) {
    char buffer[1024];
    ssize_t count = recv(player.socket, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (count == 0) { disconnectPlayer(game, player, "Cliente cerro la conexion"); return; }
    if (count < 0) {
        if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
            disconnectPlayer(game, player, "Error de recepcion");
        return;
    }
    player.input.append(buffer, static_cast<size_t>(count));
    size_t newline;
    while ((newline = player.input.find('\n')) != string::npos) {
        if (newline > 256) { disconnectPlayer(game, player, "Mensaje demasiado largo"); return; }
        string command = player.input.substr(0, newline);
        player.input.erase(0, newline + 1);
        if (!command.empty() && command.back() == '\r') command.pop_back();
        processCommand(game, player, command);
        if (player.socket < 0 || game.finished) return;
    }
    if (player.input.size() > 256) disconnectPlayer(game, player, "Mensaje demasiado largo");
}

// crea socket el punto de comunicacion, asigna la ip bindea y deja escuchando
void openServer(GameState& game) {
    game.serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (game.serverSocket < 0 || game.serverSocket >= FD_SETSIZE)
        throw runtime_error("No se pudo crear un socket compatible con select.");
    int reuseAddress = 1;
    if (setsockopt(game.serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress)) < 0)
        throw runtime_error("No se pudo configurar el servidor.");
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(game.port);
    if (bind(game.serverSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0 ||
        listen(game.serverSocket, 64) < 0)
        throw runtime_error("No se pudo escuchar. Comprueba que GAME_PORT este libre.");
}
// acepta jugador o rechaza si el socket no es compatible, ya inicio la partida o esta lleno
void acceptPlayer(GameState& game) {
    int clientSocket = accept(game.serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        if (errno != EINTR) cerr << "Error al aceptar una conexion.\n";
        return;
    }
    if (clientSocket >= FD_SETSIZE || game.started ||
        game.players.size() >= static_cast<size_t>(game.maxTeams * game.maxPerTeam)) {
        sendAll(clientSocket, "ERROR Partida iniciada o servidor completo.\n");
        close(clientSocket);
        return;
    }
    game.players.push_back({clientSocket, game.nextPlayerId++, "", -1, "", false});
    GamePlayer& player = game.players.back();
    logEvent(game, "ID=" + to_string(player.id), -1, "CONEXION", "Socket aceptado");
    sendMessage(player, "NOMBRE Ingresa un nombre unico (letras, numeros o _; maximo 20).");
}

// ciclo principal del servidor
void runGame(GameState& game) {
    while (!game.finished) {
        if (shutdownRequested) { finishGame(game, -1, "Cierre solicitado al servidor."); break; }
        for (GamePlayer& player : game.players) {
            if (game.finished) break;
            if (player.sendFailed) disconnectPlayer(game, player, "Error de envio o cliente que no lee");
        }
        for (size_t index = 0; index < game.players.size();) {
            if (game.players[index].socket < 0) {
                game.players.erase(game.players.begin() + index);
            } else {
                ++index;
            }
        }
        if (game.finished) break;
        // select identifica los sockets con datos disponibles. Un jugador esperando no bloquea a los demas
        fd_set readSockets;
        FD_ZERO(&readSockets);
        FD_SET(game.serverSocket, &readSockets);
        int maxSocket = game.serverSocket;
        for (const GamePlayer& player : game.players) {
            FD_SET(player.socket, &readSockets);
            maxSocket = max(maxSocket, player.socket);
        }
        timeval timeout{0, 200000}; // Revisar solicitudes de cierre cada 200 ms.
        int readyCount = select(maxSocket + 1, &readSockets, nullptr, nullptr, &timeout);
        if (readyCount < 0) {
            if (errno == EINTR) continue;
            throw runtime_error("Error esperando mensajes de los clientes.");
        }
        for (GamePlayer& player : game.players) {
            if (game.finished) break;
            if (FD_ISSET(player.socket, &readSockets)) receiveCommands(game, player);
        }
        // Aceptar al final para evitar modificar el vector mientras se recorre.
        if (!game.finished && FD_ISSET(game.serverSocket, &readSockets)) acceptPlayer(game);
    }
}
// configuramos el servidor, lanza la partida y libera los recursos al terminar
int main() {
    loadEnv();
    clearScreen();
    struct sigaction action{};
    action.sa_handler = requestShutdown;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGTERM, &action, nullptr);
    GameState game;
    int result = 0;
    try {
        game.port = gameNumber(getEnvVar("GAME_PORT"), 1024, 65535, "GAME_PORT");
        game.minTeams = gameNumber(getEnvVar("GAME_MIN_TEAMS"), 2, 16, "GAME_MIN_TEAMS");
        game.maxTeams = gameNumber(getEnvVar("GAME_MAX_TEAMS"), game.minTeams, 16, "GAME_MAX_TEAMS");
        game.positions.assign(game.maxTeams, 0);
        game.minPerTeam = gameNumber(getEnvVar("GAME_MIN_PER_TEAM"), 1, 32, "GAME_MIN_PER_TEAM");
        game.maxPerTeam = gameNumber(getEnvVar("GAME_MAX_PER_TEAM"), game.minPerTeam, 32, "GAME_MAX_PER_TEAM");
        game.goal = gameNumber(getEnvVar("GAME_BOARD_GOAL"), 1, 1000000, "GAME_BOARD_GOAL");
        game.diceFaces = gameNumber(getEnvVar("GAME_DICE_FACES"), 1, 1000, "GAME_DICE_FACES");
        string path = getEnvVar("GAME_LOG_FILE");
        if (path.empty()) throw runtime_error("GAME_LOG_FILE no puede estar vacia.");
        game.logFile.open(path, ios::app);
        if (!game.logFile) throw runtime_error("No se pudo abrir GAME_LOG_FILE: " + path);
        openServer(game);
        logEvent(game, "SERVIDOR", -1, "APERTURA", "Puerto " + to_string(game.port));
        cout << "========================================" << endl
             << "          SERVIDOR DEL JUEGO" << endl
             << "========================================" << endl
             << "Puerto: " << game.port << endl
             << "Ctrl+C para cerrar el servidor." << endl << endl;
        runGame(game);
        for (GamePlayer& player : game.players) disconnectPlayer(game, player, "Cierre del servidor");
        logEvent(game, "SERVIDOR", -1, "CIERRE", "Recursos liberados");
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        result = 1;
    }
    for (GamePlayer& player : game.players) if (player.socket >= 0) close(player.socket);
    if (game.serverSocket >= 0) close(game.serverSocket);
    if (game.logFile.is_open()) game.logFile.close();
    return result;
}
