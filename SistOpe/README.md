# SistOpe

Proyecto del curso **INFO198 - Sistemas Operativos**.

## Propósito de la aplicación

SistOpe es un sistema de consola desarrollado en C++ que en el Menú principal,
centraliza distintas funcionalidades bajo un esquema de perfiles de usuario con autenticación.

A partir de esta entrega, el sistema:

- Persiste la información de usuarios y perfiles en formato **binario** , en lugar del formato de texto delimitado por `;` usado en la Entrega 1.
- Se autentica al arrancar mediante argumentos de ejecución (`-u`, `-p`, `-f`).
- Presenta un menú dinámico de 7 opciones donde el acceso a cada opción depende del perfil del usuario autenticado (la opción 1 es exclusiva del perfil `ADMIN`).
- Reutiliza el módulo de Administración de Usuarios y Perfiles construido en la Entrega 1.

## Funcionalidades

El menú principal cuenta con siete opciones:

1. Administración de usuarios y perfiles
2. Multiplicación de matrices NxM
3. Juego
4. ¿Es palíndromo?
5. Calcular f(x)
6. Conteo sobre texto
7. Conteo sobre archivo

La opción `0` permite volver o salir de los menús.

### Administración de usuarios y perfiles 
- Permite crear, eliminar y listar usuarios y perfiles del sistema. Solo accesible para el perfil ADMIN.
### Multiplicación de matrices NxM 
- Recibe dos archivos de texto con matrices (A y B) y un separador, valida que las dimensiones permitan la multiplicación y muestra el resultado.
### Juego 
- Opción reservada, actualmente "en construcción" (sin funcionalidad definida aún).
### ¿Es palíndromo? 
- Abre una interfaz donde se ingresa un texto y se ofrecen dos acciones: Validar (comprueba si el texto es palíndromo) o Cancelar (vuelve al menú).
### Calcular f(x) 
- Pide un valor real de X y calcula f(x) = x² + 2x + 8, mostrando el resultado; incluye una opción Volver.
### Conteo sobre texto
- Toma el archivo indicado en el argumento -f al ejecutar el programa y entrega un resumen: cantidad de vocales, consonantes, caracteres especiales y palabras; incluye opción Volver.
### Conteo sobre archivo 
- Igual al conteo anterior, pero permite ingresar manualmente la ruta de cualquier archivo a analizar (no depende del -f inicial).



## Compilar
El proyecto se compila con un compilador C++
Para compilar el proyecto, ubicarse en la carpeta raíz del proyecto SistOpe, donde se encuentra el archivo Makefile, y ejecutar:

```bash
make clean
make
```

## Ejecutar
Una vez compilado el proyecto, el programa principal recibe:

| Argumento | Descripción |
|-----------|-------------|
| `-u` | Nombre de usuario |
| `-p` | Contraseña |
| `-f` | Ruta del archivo de trabajo |

por ejemplo:

```bash
./target/sistope -u lvc -p 1001 -f "db/entrada.txt"
```

## Variables de entorno

El proyecto usa un archivo `.env` en la raíz, con las siguientes variables:

| Variable      | Descripción                                    |
|---------------|-------------------------------------------------|
| `USER_FILE`   | Ruta al archivo de texto donde se guardan los usuarios |
| `PERFIL_FILE` | Ruta al archivo de texto donde se guardan los perfiles |
| `ADMIN_PROGRAM`   | Ruta al ejecutable encargado de administrar los usuarios y perfiles del sistema |
| `MATRIX_MULTIPLICATION_PROGRAM` | Ruta al ejecutable que realiza la multiplicación de matrices |
| `GAME_PROGRAM`   | Ruta al ejecutable correspondiente al juego del sistema|
| `PALINDROME_PROGRAM` | Ruta al ejecutable que permite trabajar con la funcionalidad de palíndromos |
| `FX_PROGRAM`   | Ruta al ejecutable que realiza el cálculo de la función \(f(x)=x^2+2x+8\) |
| `COUNT_PROGRAM` | Ruta al ejecutable encargado de realizar los conteos sobre textos y archivos. |
Formato de `.env`:
```
USER_FILE=db/USUARIOS.TXT
PERFIL_FILE=db/PERFILES.TXT
ADMIN_PROGRAM=target/util/admin
MATRIX_MULTIPLICATION_PROGRAM=target/util/matrix_multiplication
GAME_PROGRAM=target/util/game
PALINDROME_PROGRAM=target/util/palindrome
FX_PROGRAM=target/util/fx
COUNT_PROGRAM=target/util/count
```
## LIBROS
Todos los libros que se utilizan fueron obtenidos a traves de https://www.gutenberg.org/
50 Mb de libros en formato .txt fueron guardados en la carpeta LIBROS dentro de db.
