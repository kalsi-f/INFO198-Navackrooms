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
(`USUARIOS.TXT` y `PERFILES.TXT`), y se mantiene además en memoria durante la
ejecución del programa para no tener que releer el archivo en cada
operación.

## Notas de uso

- En el menú principal la opción `0` sale del programa; en los submenús vuelve atrás.
- Los campos no pueden contener `;` porque es el separador de los archivos.
- Las opciones de un perfil deben ser números no negativos.

## Cómo ejecutar

> **Nota:** este proyecto fue diseñado y probado en **Linux**. Por lo que no está
> garantizado que compile o funcione correctamente en Windows sin un
> entorno compatible. Se recomienda ejecutar en Linux, macOS, o en Windows
> a través de **WSL (Windows Subsystem for Linux)**.

### Compilar y ejecutar

Desde la raíz del proyecto (donde está el `Makefile` y el `.env`):

```bash
make run
```
Esto compila todos los `.cpp` dentro de `src/` (detectados automáticamente)
y ejecuta el binario resultante (`sistope`).

## Variables de entorno

El proyecto usa un archivo `.env` en la raíz, con las siguientes variables:

| Variable      | Descripción                                    |
|---------------|-------------------------------------------------|
| `USER_FILE`   | Ruta al archivo de texto donde se guardan los usuarios |
| `PERFIL_FILE` | Ruta al archivo de texto donde se guardan los perfiles |

Formato de `.env`:
```
USER_FILE=db/USUARIOS.TXT
PERFIL_FILE=db/PERFILES.TXT
```

Estas variables se cargan al entorno real del proceso al iniciar el
programa (`loadEnv()`), y se leen desde cualquier parte del código con
`getEnvVar("USER_FILE")` / `getEnvVar("PERFIL_FILE")`.


