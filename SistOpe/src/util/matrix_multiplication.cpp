#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void printMatrix(string variableName, vector<vector<int>> matrix) {
    cout << variableName << " = " << endl;
    
    for (vector<int>& row : matrix) {
        for (int& value : row) {
            cout << value << ", ";
        }
        cout << endl;    
    }
    cout << endl;
}

int findSeparator(string s, size_t start, char separator) {
    size_t i = start;
    while (i < s.size() && s[i] != separator) 
        i++;
    return i;
}

vector<vector<int>> loadMatrix(string filePath, char separator) {
    vector<vector<int>> matrix;
    
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo: " << filePath << endl;
        return matrix; 
    }

    string line;

    while (getline(file, line)) {
        vector<int> row;
        size_t prevPos = 0;
        size_t pos = 0;
        while (prevPos < line.size()) {
            pos = findSeparator(line, prevPos, separator);

            try {
                row.push_back(stoi(line.substr(prevPos, pos-prevPos+1)));
                prevPos = pos+1;
            }
            catch (...) {
                cerr << "Error: Matriz invalida: Se encontro un valor no numerico entero. " << filePath << endl;
                
                return matrix = {};
            }
        }
        
        matrix.push_back(row);

        if (
            matrix.size() >= 2 && 
            matrix[matrix.size()-1].size() != matrix[matrix.size()-2].size()
        ) {
            cerr << "Error: Matriz invalida: las filas tienen tamaños distintos. " << filePath << endl;
            cout << "t0: " << matrix[0].size() << " t1: " << matrix[1].size();
            return matrix = {};
        }

    }

    if (matrix.empty()) {
        cerr << "Error: Matriz vacia. " << filePath << endl;
        return matrix = {};
    }

    cout << filePath << ": Matriz cargada." << endl;

    return matrix;

}

vector<vector<int>> multiplyMatrix(vector<vector<int>> matrixA, vector<vector<int>> matrixB) {
    vector<vector<int>> result(matrixA.size());
    if (matrixA.size() >= 1 && matrixB.size() >= 1) {
        if (matrixA[0].size() == matrixB.size()) {
            for (size_t i = 0; i < matrixA.size(); i++) {
                vector<int> row(matrixB[0].size());
                
                for (size_t j = 0; j < matrixB[0].size(); j++)
                    row[j] = 0;

                for (size_t j = 0; j < matrixB[0].size(); j++) {
                    for (size_t k = 0; k < matrixA[0].size(); k++) {
                        row[j] += matrixA[i][k] * matrixB[k][j];
                    }
                }

                result[i] = row;
            }
        }
        else {
            cerr << "Error: el numero de columnas de la matriz A es distinto al numero de filas de la matriz B." << endl;
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Error: Numero de argumentos invalido." << endl;
        cout << "Uso: " << argv[0] << " <RUTA_MATRIZ_A> <RUTA_MATRIZ_B> <SEPARADOR_COLUMNA>" << endl; 
        return 1;
    }

    if (argv[3][1] != '\0') {
        cerr << "Error: El separador debe ser un caracter unico." <<  endl;
        cout << "Uso: " << argv[0] << " <RUTA_MATRIZ_A> <RUTA_MATRIZ_B> <SEPARADOR_COLUMNA>" << endl; 
        return 2;
    }

    cout << "Usando separador: " << argv[3][0] << endl;

    vector<vector<int>> matrixA = loadMatrix(argv[1], argv[3][0]);


    vector<vector<int>> matrixB = loadMatrix(argv[2], argv[3][0]);

    vector<vector<int>> matrixC = multiplyMatrix(matrixA, matrixB);

    if (!matrixC.empty()) {
        printMatrix("A", matrixA);
        printMatrix("B", matrixB);
        printMatrix("C", matrixC);
    }

    cout << "Presione ENTER para salir...";
    cin.get();
    return 0;
}