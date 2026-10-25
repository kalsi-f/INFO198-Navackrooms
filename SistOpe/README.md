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
- Procesa en paralelo todos los libros de `BOOKS_DIR` usando una cola de trabajo, `N_THREADS` hilos y semáforos, y genera el índice invertido (`INDEX_FILE`) y el mapeo id ↔ libro (`BOOK_MAP_FILE`). Solo accesible para el perfil ADMIN. Ver la sección [Índice invertido paralelo](#índice-invertido-paralelo).



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
| `INDEX_PROGRAM` | Ejecutable que crea el índice invertido |
| `BOOKS_DIR` | Carpeta con los libros `.txt` a indexar |
| `INDEX_FILE` | Ruta del índice invertido generado (`indice_invertido.idx`) |
| `BOOK_MAP_FILE` | Ruta del archivo con la asociación `id;nombre_libro` |
| `N_THREADS` | Cantidad de hilos trabajadores, entero mayor a 0 |

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
INDEX_PROGRAM=target/util/indexer
BOOKS_DIR=db/LIBROS
INDEX_FILE=db/indice_invertido.idx
BOOK_MAP_FILE=db/libros.map
N_THREADS=4
```

## Índice invertido paralelo

Un índice invertido guarda, para cada palabra, en qué libros aparece y cuántas veces. Se genera desde la opción 8 del menú o ejecutando directamente, desde la carpeta `SistOpe`:

```bash
./target/util/indexer
N_THREADS=8 ./target/util/indexer      # sobreescribe una variable del .env
```

### Archivos generados

`INDEX_FILE`, una palabra por línea, en orden alfabético, con `(id_libro,veces)` por cada libro donde aparece:
```
quijote;(8,2241);(20,1)
señor;(8,1065);(15,44);(16,28);(19,778);(20,527)
```

`BOOK_MAP_FILE`, la asociación id ↔ libro. Los ids se asignan por orden alfabético del nombre, así que se mantienen entre ejecuciones:
```
8;DonQuijote.txt
20;LosmiserablesTOMO2.txt
```

### Flujo: cola → threads → semáforos → resultados → índice

1. **Validación.** Se leen y validan `BOOKS_DIR`, `INDEX_FILE`, `BOOK_MAP_FILE` y `N_THREADS` (entero > 0). Se verifica que la carpeta exista y se pueda leer, y se listan los `.txt`. Si algo falla, se informa y no se procesa nada.
2. **Cola.** Cada libro (`id`, nombre, ruta, tamaño) se agrega a una cola de trabajo compartida, ordenada de mayor a menor tamaño para que los libros grandes no queden para el final.
3. **Threads.** Se crean `N_THREADS` hilos trabajadores (si hay más hilos que libros, se usan tantos hilos como libros). Cada hilo repite: sacar un libro de la cola → contar sus palabras en un mapa **local** → incorporar ese mapa al índice global. Cuando la cola está vacía, el hilo termina.
4. **Semáforos.** Hay tres recursos compartidos y cada uno tiene un semáforo binario (`sem_t` inicializado en 1):

   | Sección crítica | Dónde | Qué evita |
   |---|---|---|
   | Sacar de la cola (`empty` + `front` + `pop`) | `WorkQueue::tryPop` | que dos hilos tomen el mismo libro y otro quede sin procesar |
   | Agregar al índice global | `InvertedIndex::merge` | que inserciones simultáneas corrompan el `map` o pierdan palabras |
   | Escribir en consola | `worker` | que los mensajes de distintos hilos se mezclen |

   El conteo de palabras (la parte pesada) se hace **fuera** de las secciones críticas, sobre un mapa privado de cada hilo. Así los hilos cuentan en paralelo y entran al semáforo del índice una sola vez por libro.
5. **Resultados.** El `main` espera a todos los hilos con `join()`. Que la cola esté vacía no basta, porque puede haber hilos todavía contando el último libro que sacaron.
6. **Índice.** Con todos los hilos terminados se escribe `INDEX_FILE` (ya sin concurrencia, por lo que no se necesita semáforo) y se informa la cantidad de palabras y el tiempo.

### Organización del código

| Archivo | Responsabilidad |
|---|---|
| `src/util/indexer.cpp` | Orquesta el flujo: configuración, cola, hilos, `join`, escritura |
| `lib/index/IndexConfig.cpp` | Lee y valida las variables de entorno |
| `lib/index/BookScanner.cpp` | Valida la carpeta, lista los libros, asigna ids, escribe `BOOK_MAP_FILE` |
| `lib/index/Tokenizer.cpp` | Cuenta las palabras de un libro |
| `lib/index/WorkQueue.cpp` | Cola de trabajo protegida con semáforo |
| `lib/index/InvertedIndex.cpp` | Índice global protegido con semáforo; escribe `INDEX_FILE` |
| `lib/index/Worker.cpp` | Función que ejecuta cada hilo |

### Qué se considera palabra

Se pasa todo a minúsculas y cualquier carácter que no sea letra o número separa palabras. Las letras con tilde y la `ñ` (2 bytes en UTF-8, primer byte `0xC3`) se tratan como letras, siguiendo el mismo criterio que el módulo de conteo (`CountSystem`). Las comillas tipográficas y guiones largos separan palabras.

Limitaciones conocidas: `don't` se cuenta como `don` y `t`; los números también se cuentan como palabras; el texto de licencia de Project Gutenberg incluido en cada libro también se indexa.

### Verificación

El índice generado es idéntico (byte a byte) con 1, 2, 4, 8 y 100 hilos al de una versión secuencial de referencia. Con los 46 libros, 1 hilo tarda ~2,9 s y 4 hilos ~1 s.

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





