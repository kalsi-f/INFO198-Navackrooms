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

## LIBROS
Todos los libros que se utilizan fueron obtenidos a traves de https://www.gutenberg.org/

## (esto lo sacamos despues, es para que vean que falta nomas, lo de los libros se queda)
## Decisiones tomadas
se modificó la persistencia de usuarios y perfiles para
leer y escribir el struct completo directamente mediante archivos binarios,
siguiendo la logica presentada en el ejemplo de referencia del
entregable. 
se cambiaron funciones porque antes habia que trabajar con el formato de texto ahora es màs directo
Tambien se creo el ejecutable para adminusuarios y sea llamado de esa forma

Falta hacer lo de matrices, y  
(4) ¿es palíndromo?, debe abrir una interfaz para escribir un texto, y contener dos
opciones
(1) Validar (la cual debe validar si el texto ingresado es o no palíndromo),
(2) cancelar

(5) Calcular f(x)=x*x + 2x + 8, debe abrir una interfaz que permita indicar X y entregue la
especificac