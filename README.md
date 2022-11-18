# TC1004B.501_E8
* Luis Felipe Hernández Flores A01735939
* Daniela Ruiz Arenas A01732921 
* André Rossell Manrique A01736035

Repositorio para el trabajo del Equipo 8 a lo largo del desarrollo del reto para la UF Implementación del Internet de las Cosas, en ITESM Campus Puebla.
## Introducción
Para la situación problema de esta Unidad de Formación, se requiere proponer una aplicación para dispositivos móviles que lea desde una base de datos en tiempo real hosteada en Firebase la información de 5 variables físicas y a su vez sea capaz de publicar en la misma el valor ingresado en dos campos de entrada, uno llamado _usuario_ y otro llamado _numero_; las 5 variables físicas serán publicadas aprovechando las capacidades de conexión a internet mediante WiFi de la placa ESP32, a la que se conectarán diversos sensores para medición de magnitudes y detección del entorno y un display de 7 segmentos que imprima el número publicado por el usuario de la aplicación previamente mencionada, con la validación necesaria para detectar si el valor no puede ser representado con los 7 segmentos. Finalmente, también se desarrolló un programa en Python para visualizar la información de la base y señalizar mediante una modificación al campo de _usuario_ si detecta que el valro de _numero_ es inválido (al ser un display de 7 segmentos, no puede representar un número mayor a 9). 
## Desarrollo
### Puesta en marcha del ESP32 en el IDE Arduino
Para poder utilizar los sensores disponibles para el ESP32, necesitamos programarlo a través del IDE Arduino, con la finalidad de manejar la información recabada a través de los sensores y enviarla a la base de datos utilizando la conexión a internet de la placa. 
### Configuración de Firebase
La base de datos será manejada a través de la plataforma de Firebase, en la cual debemos configurar un proyecto antes de poder publicar o leer información. 

Estructura de la base de datos:

![imagen](https://user-images.githubusercontent.com/92490116/202615924-237e187d-4fc3-46fa-a795-56730ca2f5e9.png)

### Configuración de los sensores en el ESP32
Una vez que el ESP32 está correctamente conectado con el entorno de desarrollo y tenemos una base de datos habilitada, podemos pasar a conectar la placa con los sensores mediante la protoboard. Una vez habilitados los sensores, pueden comenzar a recabar mediciones que serán públicadas; las variables físicas a publicar en la base de datos son: 
1. Temperatura (en Celsius y Fahrenheit)
2. Humedad
3. Distancia 
4. Flama 
5. Movimiento
### Desarrollo de código para interacción con IoT
Se produjeron diversos códigos para cumplir con los objetivos del sistema y todas sus integraciones multiplataforma. Todo el código está disponible en sus respectivos directorios en este repositorio y su documentación se encuentra disponible a detalle, junto con los pasos de cada uno de los anteriores apartados, en la [wiki](https://github.com/andrerossellm/TC1004B.501_E8/wiki)
## Resultados
El sistema final funciona de manera adecuada

### Implementación en Python
* Cuando el numero es menor que 9:
  Entrada
![image](https://user-images.githubusercontent.com/92490116/202628178-6f17d2d4-4dbd-4dfe-adf2-3aa2af377f6e.png)
  Ejecución
![image](https://user-images.githubusercontent.com/92490116/202628245-f9657065-dabd-4724-89c8-e064ba9d05dc.png)
  Salida
![image](https://user-images.githubusercontent.com/92490116/202628389-20a7dd72-dbc3-43d6-a46d-3cbaa719a072.png)
* Cuando el numero es mayor que 9:
  Entrada
![image](https://user-images.githubusercontent.com/92490116/202628495-90e6d41c-1c84-4962-9e75-3329d51c1879.png)
  Ejecución
![image](https://user-images.githubusercontent.com/92490116/202628559-9818972f-1b64-4819-ae1a-5660918c5452.png)
  Salida
![image](https://user-images.githubusercontent.com/92490116/202628596-e6d8e395-828d-484e-aacb-c1407710ec06.png)



Y, como se puede observar y recrear, reacciona de acuerdo a lo que se esperaba desde la etapa de diseño. 

## Conclusiones
Desarrollar un sistema multiplataforma de IoT nos permite identificar claramente la aplicación de conceptos revisados durante toda la unidad de formación; el punto que resalta para nosotros es la capacidad de transportar información haciendo uso de diversos tipos de dispositivos, que se encuentran interconectados y que pueden reaccionar a los comportamientos el uno del otro. No solo hablamos de información portable y accesible, también de un universo de dispositivos que pueden disponer de ella, con sistemas que modelen decisiones a partir de la misma, de esta forma es que el internet de las cosas empieza a generar oportunidad para la automatización de procesos e interconexión cotidiana. Sistemas que hoy aún parecen experimentales como de conducción autónoma se beneficiarían enormemente de formar parte de una red con información en tiempo real respecto a las condiciones de tráfico vehicular o de semáforos. Con esto en mente, nuestro proyecto demuestra el potencial para este tipo de sistemas que podrían servir, por ejemplo, para monitorear activamente condiciones en un laboratorio. Finalmente, nos quedamos satisfechos de haber aplicado correctamente los contenidos revisados en la UF para concretar un proyecto integrador exitosamente, que prueba en tiempo real las interacciones posibles por las infraestructuras de red. 

