# Proyecto - Entrega 1 

## Sistemas Operativos 

Dr. Luis Veas-Castillo 

Recuerden, son 4 o 5 personas por grupo. 

Fecha de entrega: 20/08/2026 antes de clases (se revisará en clases) Forma de entrega: correo electrónico a luis.veasc@inf.uach.cl, repositorio en github (permisos a luis.veasc@gmail.com),debe respetar el siguiente formato 

Asunto: “INFO198 Sistemas Operativos - Proyecto - Entrega 1” 

En el cuerpo del correo deben: 

Entregar la lista al repositorio de github lista de integrantes del grupo Grupo X, donde X es el id de su grupo 

## Descripción del trabajo 

Se trabajará en el sistema “SistOpe”, el cual se irá trabajando de manera transversal durante todo el curso, en esta primera entrega debemos crear el módulo de sistema llamado “Administrador de Usuarios y Perfiles”, el cual debe ser capaz de crear, eliminar y listar los usuarios y perfiles. Todos estos usuarios deben ser almacenados en un archivo de texto llamado “USUARIOS.TXT” y los perfiles en “PERFILES.TXT”. A continuación se describen los hitos de la revisión. 

- Respetar fecha de entrega 

- Respetar formatos de email 

- Debe trabajar con github 

- Su github debe contener un README que explique: (1) propósito de la aplicación, (2) como se debe ejecutar (3) descripción de las variables de entorno 

- Debe trabajar con variables de entorno en un archivo .env, por ahora solo tendrá la variables USER_FILE, PERFIL_FILE 

- Los identificadores de las opciones de menú deben ser internamente tratadas con números 

- La opción 0 permite Salir 

- Debe trabajar con struct 

- Su “struct” debe contener: id, nombre, username, password, perfil 

- El sistema debe permitir Salir con 0 

- El sistema debe permitir ingresar usuarios, al ingresar estos usuarios deben ser cargados en el “struct” de usuarios, 

- Debe crear un “struct” lista de usuarios, que permitirá mantener a todos los usuarios en memoria. 

- Si da a la opción guardar (en la ventana admin-ingresar), debe agregar el usuario creado a la lista de usuarios y debe agregar un registro al final del archivo antes mencionado. 

- Debe permitir listar los usuarios del sistema, si los datos ya están cargados en memoria, los lee de la lista de struct de usuarios, de lo contrario, lee el archivo 

- Los perfiles puedes ser “GENERAL” y “ADMIN” 

- Debe permitir eliminar un usuario por su ID, si el usuario a eliminar es perfil “ADMIN”, debe enviar un mensaje de alerta señalando el posible error 

- Deben crear el PERFIL de usuarios (ingresar, eliminar, y listar con la misma lógica anterior), es decir, debe tener la variable de entorno PERFIL_FILE. Un ejemplo de esta estructura es un [ string, array de enteros ] 

ADMIN;0,1,2,3,4 GENERAL;0,1,3 

Donde los números representan las futuras opciones de menú que podrá manipular 

Notas: puede entregar atrasado, cada día de atraso es 1 punto menos, el día de atraso parte el mismo 20/08/2026, antes de clases. 

Algunas referencias: 

<u>https://www.w3schools.com/cpp/cpp_structs.asp</u> 

<u>https://stackoverflow.com/questions/74452768/c-save-and-load-structures-to-file</u> 

<u>https://stackoverflow.com/questions/631664/accessing-environment-variables-in-c</u> 

<u>https://www.geeksforgeeks.org/cpp/how-to-create-an-array-of-structs-in-cpp/</u> 



<!-- Start of picture text -->
Ingreso de usuarios<br>Id: 1<br>Nombre: Luis Veas<br>username: lve<br>MéduloP . Le . password: xxxxxx<br> - Gestidn de Usuarios perfil; ADMIN<br>0) Salir 1) guardar 2) cancelar<br>1) Ingresar Usuarios<br>2) Listar Usuarios Opcién : 1<br>3) Eliminar Usuarios<br>Opcién ; O Lista de usuarios<br>Id Nombre Perfil<br>1 Luis Veas ADMIN<br>2 Moria A GENERAL<br>3 Pedro GENERAL<br>Eliminar Usuarios . .<br>ID usuario a borrar: X 1) para Volver : 1<br>1) guardar 2) cancelar<br>Opciin ; 1<br><!-- End of picture text -->

