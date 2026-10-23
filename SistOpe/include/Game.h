#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <random>
#include <string>
#include <vector>

// la idea de guardarl en estructuras es que se leen en el codigo y se validan, para pasarlas a las estructuras

// configuracion de jugador
struct GamePlayer {
    int socket;
    int id;
    std::string name;
    int team = -1;
    std::string input;
    bool sendFailed = false;
};

// configuracion de partida
struct GameState {
    int serverSocket = -1;
    int port = 0;
    int minTeams = 0;
    int maxTeams = 0;
    int minPerTeam = 0;
    int maxPerTeam = 0;
    int goal = 0;
    int diceFaces = 0;
    int nextPlayerId = 1;
    int currentTurn = -1;
    std::vector<long long> positions;
    bool started = false;
    bool finished = false;
    std::vector<GamePlayer> players;
    std::ofstream logFile;
    std::mt19937 randomEngine{std::random_device{}()};
};

// para verificar variables del juego, tamaño equipos y eso
int gameNumber(const std::string& value, int minimum, int maximum,
               const std::string& name);

//enviar mensajes a traves de los sockets, repitiendo hasta enviarlo completo
bool sendAll(int socket, const std::string& message);

#endif
