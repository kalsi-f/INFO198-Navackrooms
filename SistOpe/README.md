# SistOpe

Proyecto del curso **INFO198 - Sistemas Operativos**.

## Propósito de la aplicación

SistOpe es un sistema de consola desarrollado en C++ que, en esta primera
entrega, implementa el módulo **Administrador de Usuarios y Perfiles**.

El sistema permite:

- Crear, listar y eliminar **usuarios**, cada uno con un `id`, nombre,
  username, password y un perfil asociado.
- Crear, listar y eliminar **perfiles**, cada uno con un nombre y una lista
  de opciones de menú permitidas (por ejemplo `ADMIN;0,1,2,3,4`).

Toda la información se persiste en archivos de texto plano
(`USERS.TXT` y `PROFILES.TXT`), y se mantiene además en memoria durante la
ejecución del programa para no tener que releer el archivo en cada
operación.

## Cómo ejecutar

### Requisitos

- `g++` con soporte para C++17.
- `make`.

### Compilar y ejecutar

Desde la raíz del proyecto (donde está el `Makefile` y el `.env`):

```bash
make run
```

Esto compila todos los `.cpp` dentro de `src/` (detectados automáticamente)
y ejecuta el binario resultante (`sistope`).

Otros comandos disponibles:

```bash
make        # solo compila, sin ejecutar
make clean  # elimina el ejecutable, para compilar desde cero
```

## Variables de entorno

El proyecto usa un archivo `.env` en la raíz, con las siguientes variables:

| Variable      | Descripción                                    |
|---------------|-------------------------------------------------|
| `USER_FILE`   | Ruta al archivo de texto donde se guardan los usuarios |
| `PERFIL_FILE` | Ruta al archivo de texto donde se guardan los perfiles |

Formato de `.env`:
```
USER_FILE=db/USERS.TXT
PERFIL_FILE=db/PROFILES.TXT
```

Estas variables se cargan al entorno real del proceso al iniciar el
programa (`loadEnv()`), y se leen desde cualquier parte del código con
`getEnvVar("USER_FILE")` / `getEnvVar("PERFIL_FILE")`.


