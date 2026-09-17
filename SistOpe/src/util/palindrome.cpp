#include <Menu.h>
#include <iostream>
#include <vector>

bool isPalindrome(string s) {
    for (size_t i = 0; i < s.size()/2; i++) {
        if (s[i] != s[s.size()-i-1]) return false;
    }
    return true;
}

int main() {
    clearScreen();

    int option = -1;
    string input;
    vector<string> options = {
        "Validar",
        "Cancelar"
    };

    while (option != 1) {
        cout << "Ingrese texto a verificar que es palindrome: ";
        getline(cin, input);

        option = simple_menu(options);

        if (option == 0) {
            if (isPalindrome(input)) cout << "Es";
            else cout << "No es";
            cout << " palindrome." << endl;
        }
    }

    return 0;
}