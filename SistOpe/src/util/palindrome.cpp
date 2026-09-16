#include <Menu.h>
#include <iostream>
#include <vector>

bool is_palindrome(string s) {
    for (size_t i = 0; i < s.size()/2; i++) {
        if (s[i] != s[s.size()-i-1]) return false;
    }
    return true;
}

int main() {
    clear_screen();

    int option = -1;
    string input;
    std::vector<string> options = {
        "Validar",
        "Cancelar"
    };

    while (option != 1) {
        std::cout << "Ingrese texto a verificar que es palindrome: ";
        getline(std::cin, input);

        option = simple_menu(options);

        if (option == 0) {
            if (is_palindrome(input)) std::cout << "Es";
            else std::cout << "No es";
            std::cout << " palindrome." << std::endl;
        }
    }

    return 0;
}