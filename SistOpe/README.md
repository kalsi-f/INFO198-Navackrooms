# SistOpe

Proyecto del curso **INFO198 - Sistemas Operativos**.

## Propósito de la aplicación

SistOpe es un sistema de consola desarrollado en C++ que en el Menú principal,
centraliza distintas funcionalidades bajo un esquema de perfiles de usuario con autenticación.

A partir de esta entrega, el sistema:

- Persiste la información de usuarios y perfiles en formato **binario** , en lugar del formato de texto delimitado por `;` usado en la Entrega 1.
- Se autentica al arrancar mediante argumentos de ejecución (`-u`, `-p`, `-f`).
- Presenta un menú dinámico de 8 opciones donde el acceso a cada opción depende del perfil del usuario autenticado (las opciones 1 y 8 son exclusivas del perfil `ADMIN`).
- Reutiliza el módulo de Administración de Usuarios y Perfiles construido en la Entrega 1.

## Funcionalidades

El menú principal cuenta con ocho opciones:

1. Administración de usuarios y perfiles
2. Multiplicación de matrices NxM
3. Juego
4. ¿Es palíndromo?
5. Calcular f(x)
6. Conteo sobre texto
7. Conteo sobre archivo
8. Crear índice invertido

La opción `0` permite volver o salir de los menús.

### Administración de usuarios y perfiles 
- Permite crear, eliminar y listar usuarios y perfiles del sistema. Solo accesible para el perfil ADMIN.
### Multiplicación de matrices NxM 
- Recibe dos archivos de texto con matrices (A y B) y un separador, valida que las dimensiones permitan la multiplicación y muestra el resultado.
### Juego 
- Carrera multiplayer por equipos mediante sockets TCP. La opción 3 abre un menú para iniciar el servidor o un cliente. Ver las reglas e instrucciones del problema 2 al final de este documento.
### ¿Es palíndromo? 
- Abre una interfaz donde se ingresa un texto y se ofrecen dos acciones: Validar (comprueba si el texto es palíndromo) o Cancelar (vuelve al menú).
### Calcular f(x) 
- Pide un valor real de X y calcula f(x) = x² + 2x + 8, mostrando el resultado; incluye una opción Volver.
### Conteo sobre texto
- Toma el archivo indicado en el argumento -f al ejecutar el programa y entrega un resumen: cantidad de vocales, consonantes, caracteres especiales y palabras; incluye opción Volver.
### Conteo sobre archivo 
- Igual al conteo anterior, pero permite ingresar manualmente la ruta de cualquier archivo a analizar (no depende del -f inicial).
### Crear índice invertido
- Construye en paralelo, con una cola de trabajo, threads y semáforos, un índice con las palabras de todos los libros y cuántas veces aparece cada una en cada libro. Solo accesible para el perfil ADMIN. Ver la sección "Índice invertido" más abajo.



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
| `GAME_PROGRAM` | Menú del juego usado por el sistema |
| `GAME_SERVER_PROGRAM` | Ejecutable del servidor|
| `GAME_CLIENT_PROGRAM` | Ejecutable del cliente|
| `GAME_LOG_FILE` | Ruta al archivo donde se guardan los logs |
| `GAME_PORT` | `5000` | Puerto TCP entero |
| `GAME_MIN_TEAMS` |  Mínimo de equipos para iniciar, entre 2 y 16 |
| `GAME_MAX_TEAMS` |  Máximo de equipos, entre el mínimo y 16 |
| `GAME_MIN_PER_TEAM` |  Mínimo por equipo, entre 1 y 32 |
| `GAME_MAX_PER_TEAM` |  Máximo por equipo, entre el mínimo y 32 |
| `GAME_BOARD_GOAL` |  Meta entera entre 1 y 1000000 |
| `GAME_DICE_FACES` | Caras del dado, entre 1 y 1000 |
| `INVERTED_INDEX_PROGRAM` | Ruta al ejecutable que crea el índice invertido (opción 8) |
| `INVERTED_INDEX_FILE` | Ruta del archivo donde se guarda el índice invertido |
| `BOOK_MAP_FILE` | Ruta del archivo con la asociación `id;nombre` de cada libro |
| `BOOKS_DIR` | Carpeta con los libros a procesar |
| `N_THREADS` | Cantidad de threads trabajadores, entero entre 1 y 64 |

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
GAME_SERVER_PROGRAM=target/util/game_server
GAME_CLIENT_PROGRAM=target/util/game_client
GAME_LOG_FILE=db/game.log
GAME_PORT=5000
GAME_MIN_TEAMS=2
GAME_MAX_TEAMS=4
GAME_MIN_PER_TEAM=2
GAME_MAX_PER_TEAM=4
GAME_BOARD_GOAL=30
GAME_DICE_FACES=6
INVERTED_INDEX_PROGRAM=target/util/inverted_index
INVERTED_INDEX_FILE=db/indice_invertido.idx
BOOK_MAP_FILE=db/libros.map
BOOKS_DIR=db/LIBROS
N_THREADS=4
```

## Índice invertido

La opción 8 del menú principal, **Crear índice invertido** (exclusiva del perfil `ADMIN`), lanza el ejecutable `target/util/inverted_index`. Este procesa todos los libros de `BOOKS_DIR` y construye un índice invertido: para cada palabra, en qué libros aparece y cuántas veces.

El código está en:

| Archivo | Contenido |
|---------|-----------|
| `src/util/inverted_index.cpp` | `main`: valida las variables de entorno y coordina los pasos |
| `include/InvertedIndex.h` | Structs `Book` (elemento de la cola) e `IndexState` (estado compartido) |
| `lib/util/InvertedIndex.cpp` | Cola de trabajo, threads, semáforos y armado del índice |
| `lib/util/CountSystem.cpp` | `countWordFrequency`: cuenta las apariciones de cada palabra de un libro |

### Funcionamiento: cola → threads → semáforos → resultados → índice invertido

1. **Validación.** Antes de empezar se validan las variables de entorno (`N_THREADS` debe ser un entero entre 1 y 64) y la carpeta de libros: que exista, que sea una carpeta, que se pueda leer y que tenga libros. Si hay un problema se informa y no se inicia el procesamiento.
2. **Identificación de los libros.** Cada archivo de la carpeta recibe un id numérico único (1..N, en orden alfabético). La asociación `id;nombre` se guarda en `BOOK_MAP_FILE`.
3. **Cola de trabajo.** El thread principal es el productor: agrega cada libro a una cola compartida (`queue<Book>`).
4. **Threads trabajadores.** Se crean `N_THREADS` threads (`std::thread`). Cada uno repite: sacar un libro de la cola, leerlo (`readTextFile`), contar sus palabras (`countWordFrequency`) e integrar el resultado al índice global. Un mismo libro nunca lo procesan dos threads, porque sacarlo de la cola es una sección crítica.
5. **Semáforos** (`<semaphore.h>`). Protegen los recursos compartidos:

    | Semáforo | Tipo | Sección crítica que controla |
    |----------|------|------------------------------|
    | `availableBooks` | Contador, parte en 0 | Cuántos elementos hay en la cola. Un thread sin trabajo se bloquea en `sem_wait` hasta que el productor agrega algo (`sem_post`) |
    | `queueMutex` | Binario, parte en 1 | Agregar y sacar elementos de la cola |
    | `indexMutex` | Binario, parte en 1 | Integrar un resultado al índice global y actualizar los contadores de libros |
    | `outputMutex` | Binario, parte en 1 | Escribir mensajes en pantalla sin que se mezclen |

6. **Resultados parciales.** Leer y contar un libro se hace **fuera** de las secciones críticas, sobre un `map` propio de cada thread; esa es la parte que corre en paralelo. Solo la integración al índice global ocurre dentro de la sección crítica.
7. **Fin del procesamiento.** Después de los libros, el thread principal agrega una marca de fin por cada thread. Como la cola es FIFO, las marcas salen después de todos los libros: cada thread que recibe una termina. El thread principal espera a todos con `join()`; recién ahí la cola está vacía, todos los libros fueron procesados y el índice está completo. Entonces se ordena y se escribe en `INVERTED_INDEX_FILE`.

### Conteo de palabras

Se usan las mismas reglas que el conteo de las opciones 6 y 7 (CountSystem): una palabra es una secuencia de letras (incluye tildes, ü y ñ) y dígitos; cualquier otro carácter la separa. Las palabras se guardan en minúsculas, así "Corazón" y "corazón" son la misma palabra.

### Archivos generados

`BOOK_MAP_FILE`, una línea por libro:

```
1;ARoomWithAView.txt
2;AStudyInScarlet.txt
```

`INVERTED_INDEX_FILE`, una línea por palabra en orden alfabético. Cada par es `(id del libro, cantidad de apariciones)`:

```
corazón;(8,141);(15,70);(16,171);(19,92)
```

### Ejecutar

Desde el menú principal con un usuario `ADMIN`: opción 8 y luego 1. También se puede ejecutar directamente desde la carpeta SistOpe:

```bash
./target/util/inverted_index
```

La cantidad de threads se define en `.env` con `N_THREADS`. Para probar otro valor sin editar el archivo, se puede definir al ejecutar, porque las variables que ya existen en el entorno tienen prioridad sobre el `.env`:

```bash
N_THREADS=8 ./target/util/inverted_index
```

## LIBROS
Todos los libros que se utilizan fueron obtenidos a traves de https://www.gutenberg.org/
50 Mb de libros en formato .txt fueron guardados en la carpeta LIBROS dentro de db.



### Comandos del juego


| Comando | Uso |
|---|---|
| `/iniciar` | Comenzar si se cumplen los límites de equipos y jugadores |
| `/lanzar` | Tirar el dado, únicamente durante el turno propio |
| `/estado` | Consultar equipos, posiciones, meta y turno/espera |
| `/ayuda` | Mostrar comandos |
| `/salir` | Desconectarse |





