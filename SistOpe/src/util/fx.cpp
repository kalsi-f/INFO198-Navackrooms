#include <Menu.h>
#include <iostream>
#include <vector>

int f(int x){
    return x*x + 2*x + 8;
}

int main() {
    clear_screen();
    
    int option = -1;
    int x;
    string input;
    std::vector<string> options = {
        "Calcular otro numero",
        "Volver"
    };

    while (option != 1) {
        std::cout << "Ingrese número entero x = ";
        getline(std::cin, input);
        
        try {
            x = stoi(input);
        }
        catch (...) {
            display_error("No es un numero entero.");
            continue;
        }

        std::cout << "f(" << x << ") = " << f(x) << endl;

        option = simple_menu(options);
        
        if (option == 0) clear_screen();
    }

    return 0;
}