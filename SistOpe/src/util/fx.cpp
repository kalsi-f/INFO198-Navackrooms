#include <Menu.h>
#include <iostream>
#include <vector>

int f(int x){
    return x*x + 2*x + 8;
}

int main() {
    clearScreen();
    
    int option = -1;
    int x;
    string input;
    vector<string> options = {
        "Calcular otro numero",
        "Volver"
    };

    while (option != 1) {
        cout << "Ingrese número entero x = ";
        getline(cin, input);
        
        try {
            x = stoi(input);
        }
        catch (...) {
            display_error("No es un numero entero.");
            continue;
        }

        cout << "f(" << x << ") = " << f(x) << endl;

        option = simple_menu(options);
        
        if (option == 0) clearScreen();
    }

    return 0;
}