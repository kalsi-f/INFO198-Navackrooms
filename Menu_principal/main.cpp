#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]){

    //Verificacion cantidad argumentos
    if(argc != 7 || string(argv[1]) != "-u" || string(argv[3]) != "-p" || string(argv[5]) != "-f"){
        cerr <<"Error de entrada, Orden o cantidad de argumentos invalido." << endl;
        cerr << "Uso correcto: "<< argv[0] << " -u <usuario> -p <password> -f <archivo.txt>" << endl;
        return 1;
    }

    // asignacion directa
    string userName = argv[2];
    string userPassword = argv[4];
    string filePath= argv[6];
    
    //prueba
    cout << "--- INICIANDO SISTEMA ---" << endl;
    cout << "Usuario intentando entrar: " << userName << endl;
    cout << "Clave ingresada: " << userPassword << endl;
    cout << "Archivo objetivo: " << filePath << endl;
    return 0;
}