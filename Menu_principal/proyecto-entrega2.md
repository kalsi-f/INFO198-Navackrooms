# Proyecto - Entrega 2 

## Sistemas Operativos 

Dr. Luis Veas-Castillo 

Fecha de entrega: jueves 17/09/2025 antes de clases 

Forma de entrega: correo electrónico al email luis.veasc@inf.uach.cl, repositorio en github (permisos a luis.veasc@gmail.com), debe respetar el siguiente formato 

Asunto: “INFO198 Sistemas Operativos - Proyecto - 

Entrega2”En el cuerpo del correo deben: 

Entregar la lista al repositorio de github lista de integrantes del grupo 

Grupo X, donde X es el id de su grupo 

## Descripción del trabajo 

Trabajará  en  la  construcción  de  un  sistema  llamado  MENÚ  PRINCIPAL,  este menú trabajarácon perfiles de usuario, recibiendo argumentos de  ejecución  y autenticación  de usuarios.  El detalle de lo que debe contener es: 

- Respetar fecha de entrega● 

Respetar formatos de email● Debe trabajar con github 

- Su github debe contener un README que explique: (1) propósito de la aplicación, debe incorporar las nuevas funcionalidades, (2) cómo se debe ejecutar, debe compilar y ejecutar el archivo ejecutable desde consola (3) descripción de las variables de entorno 

- La opción 0 permite Salir 

- La lectura y escritura del los struct de usuario y perfiles deben realizarlo leyendo y escribiendo el struct completo, tal como lo presenté en el ejemplo (link 2 de las referencias del entregable anterior) 

- los mensajes entregados por la aplicación deben ser claros y deben proteger la integridad del sistema 

- toda variable relevante debe estar en el .env 

- cuando yo revise el sistema, este debe estar compilado 

- El sistema debe recibir argumentos de ejecución los cuales maneje los argumentos● -u (usuario) 

- -p 

- (password)● -f (file) 

### Ejemplo: ./pgm -u lvc -p 1001 -f “/home/lvc/archivo.txt” 

- el menú debe presentar 7 opciones, la opción 1 solo la puede acceder el perfil ADMIN. Además debe mostrar el nombre de usuario y su perfil en la interfaz principal 

- las opciones de menú son: (1) Admin de usuarios y perfiles (debe realizar una llamada a sistema“ADMINISTRACIÓN DE USUARIOS Y PERFILES”, que ya está construido en la entrega 1) 

(2)  Multiplica  matrices  NxM  (debe  leer  archivos  de texto en el cual están las matrices, este debe ser otro programa, que sea llamado desde el menú) Detallado más abajo. 

(3) Juego (mensaje en construcción) 

- (4) ¿es palíndromo?, debe abrir una interfaz para escribir un texto, y contener dos opciones 

   - (1) Validar (la cual debe validar si el texto ingresado es o no palíndromo), (2) cancelar 

(5) Calcular f(x)=x*x + 2x + 8, debe abrir una interfaz que permita indicar X y entregue la especificación con el cálculo con números reales, además incorporar una opción VOLVER 

(6) CONTEO SOBRE TEXTO, esta opción debe entregar una interfaz que nos indique, resumen de conteo con: cantidad de vocales, cantidad de  consonantes,  cantidad  de caracteres especiales, cantidad de palabras, además incorporar una opción VOLVER. El archivo a contar será el que ingresó en el parámetro -f 

(7) CONTEO SOBRE ARCHIVO, interfaz que le pueden pasar un  path  de  archivo  y realiza el mismo conteo que la opción (6) 

(8) importante la interfaz debe ser auto explicativa, debe tener un título, nombre  de usuarios, perfil y luego el resto del sistema 

### **Programa multiplicador de matrices** 

- Debe implementar otro programa que multiplique matrices de NxM (validar que es posible multiplicar) 

- Este  programa  recibe  como  argumento  las  rutas  (completas)  de  los  archivos A.TXT y B.TXT que contiene las matrices y el separador de los elementos en la matriz. Además, recibe el nombre de usuario y perfil, los cuales deben ser mostrados por pantalla 

- Debe Validar filas y columnas, formatos (separador) y contenido de los archivos 

a. Ejemplo: ./multi “/home/lvc/a.txt” “/home/lvc/b.txt”“#” 

- Debe buscar y dejar en una carpeta llamada LIBROS de más de 50MB libros de diferentes géneros (ciencia ficción, fantasía, drama, biografías, ciencias, naturaleza, etc), estos libros deben ser con extensión“txt” 



