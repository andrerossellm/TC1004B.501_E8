# TC1004B.501_E8
Repositorio para el trabajo del Equipo 8 a lo largo del desarrollo del reto para la UF Implementación del Internet de las Cosas, en ITESM Campus Puebla.

Luis Felipe Hernández Flores A01735939, Daniela Ruiz Arenas A01732921, André Rossell Manrique A01736035

### Sistema propuesto
Para la situación problema de esta Unidad de Formación, se requiere proponer una aplicación para dispositivos móviles que lea desde una base de datos en tiempo real hosteada en Firebase la información de 5 variables físicas y a su vez sea capaz de publicar en la misma el valor ingresado en dos campos de entrada, uno llamado _usuario_ y otro llamado _numero_; las 5 variables físicas serán publicadas aprovechando las capacidades de conexión a internet mediante WiFi de la placa ESP32, a la que se conectarán diversos sensores para medición de magnitudes y detección del entorno y un display de 7 segmentos que imprima el número publicado por el usuario de la aplicación previamente mencionada, con la validación necesaria para detectar si el valor no puede ser representado con los 7 segmentos. Finalmente, también se desarrolló un programa en Python para visualizar la información de la base y señalizar mediante una modificación al campo de _usuario_ si detecta que el valro de _numero_ es inválido (al ser un display de 7 segmentos, no puede representar un número mayor a 9). 
### Etapas principales:
* Puesta en marcha del ESP32 en el IDE Arduino
* Configuración de los sensores en el ESP32
* Configuración de Firebase
* Desarrollo de código para interacción con IoT

Para ver una descripción detallada, [visita la wiki](https://github.com/andrerossellm/TC1004B.501_E8/wiki) 
