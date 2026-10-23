#include "Game.h"
#include <cerrno>
#include <sys/socket.h>
#include <stdexcept>

using namespace std;

int gameNumber(const string& value, int minimum, int maximum,
               const string& name) {
    if (value.empty()) {
        throw runtime_error(name + " no puede estar vacio.");
    }
    for (char character : value) {
        if (character < '0' || character > '9') {
            throw runtime_error(name + " debe contener solo digitos.");
        }
    }
    long long number;
    try { number = stoll(value); }
    catch (...) { throw runtime_error(name + " esta fuera de rango."); }
    if (number < minimum || number > maximum)
        throw runtime_error(name + " debe estar entre " +
            to_string(minimum) + " y " + to_string(maximum) + ".");
    return static_cast<int>(number);
}


bool sendAll(int socket, const string& message) {
    size_t bytesSent = 0;
    while (bytesSent < message.size()) {
        ssize_t count = send(socket, message.data() + bytesSent,
                               message.size() - bytesSent, MSG_NOSIGNAL | MSG_DONTWAIT);
        if (count < 0 && errno == EINTR) continue;
        if (count <= 0) return false;
        bytesSent += static_cast<size_t>(count);
    }
    return true;
}
