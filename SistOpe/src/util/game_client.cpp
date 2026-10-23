#include "Game.h"
#include "Menu.h"
#include "config/EnvConfig.h"
#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

string promptServerAddress() {
    cout << "IP del servidor: " << flush;
    string address;
    char character;
    while (true) {
        ssize_t count = read(STDIN_FILENO, &character, 1);
        if (count <= 0) {
            throw runtime_error("No se pudo leer la IP.");
        }
        if (character == '\n') break;
        address += character;
    }
    return address;
}
// recibir mensajes
void runClient(int clientSocket) {
    while (true) {
        fd_set readSockets;
        FD_ZERO(&readSockets);
        FD_SET(clientSocket, &readSockets);
        FD_SET(STDIN_FILENO, &readSockets);
        if (select(clientSocket + 1, &readSockets, nullptr, nullptr, nullptr) < 0) {
            throw runtime_error("No se pudo esperar la entrada del juego.");
        }
        if (FD_ISSET(clientSocket, &readSockets)) {
            char response[4096];
            ssize_t count = recv(clientSocket, response, sizeof(response), 0);
            if (count == 0) {
                cout << "\nConexion finalizada por el servidor.\n";
                return;
            }
            if (count < 0) {
                throw runtime_error("Se perdio la conexion con el servidor.");
            }
            cout.write(response, count);
            cout.flush();
        }
        if (FD_ISSET(STDIN_FILENO, &readSockets)) {
            char input[1024];
            ssize_t count = read(STDIN_FILENO, input, sizeof(input));
            if (count == 0) return;
            if (count < 0) {
                throw runtime_error("No se pudo leer el teclado.");
            }
            if (!sendAll(clientSocket, string(input, static_cast<size_t>(count)))) {
                throw runtime_error("No se pudo enviar el comando.");
            }
        }
    }
}
// conexion entre cliente y server
int main() {
    loadEnv();
    clearScreen();
    cout << "========================================" << endl
         << "          CONECTARSE AL JUEGO" << endl
         << "========================================" << endl;
    int clientSocket = -1;
    int result = 0;
    try {
        int port = gameNumber(getEnvVar("GAME_PORT"), 1024, 65535, "GAME_PORT");
        string host = promptServerAddress();
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (inet_pton(AF_INET, host.c_str(), &address.sin_addr) != 1)
            throw runtime_error("IP invalida. Utiliza una direccion IPv4.");

        // Client socket sequence from the example: socket -> connect -> send/recv -> close.
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket < 0 || clientSocket >= FD_SETSIZE)
            throw runtime_error("No se pudo crear un socket compatible con select.");
        if (connect(clientSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
            throw runtime_error("No se pudo conectar. Comprueba la IP, el puerto y que el servidor este iniciado.");
        cout << endl << "Conectado a " << host << ":" << port << endl
             << "Escribe cada comando y presiona ENTER." << endl
             << "Ayuda: /ayuda    Estado: /estado    Salir: /salir" << endl << endl;
        runClient(clientSocket);
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        result = 1;
    }
    if (clientSocket >= 0) close(clientSocket);
    return result;
}
