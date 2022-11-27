#include <esp_wpa2.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>  
#include <addons/TokenHelper.h>
#include "Arduino.h"
#include "addons/RTDBHelper.h"
#include "DHT.h"



//Se declaran los pines a usar para el display
int LEDs[] = {25,16,5,18,21,3,1};
//25 g, 16 f, 5 e, 18 d, 3 c, 21 b, 1 a

// Se declaran los arreglos que forman los dígitos
int zero[] = {0, 1, 1, 1, 1, 1, 1};   // cero
int one[] = {0, 0, 0, 0, 1, 1, 0};   // uno
int two[] = {1, 0, 1, 1, 0, 1, 1};   // dos
int three[] = {1, 0, 0, 1, 1, 1, 1};   // tres
int four[] = {1, 1, 0, 0, 1, 1, 0};   // cuatro 
int five[] = {1, 1, 0, 1, 1, 0, 1};   // cinco
int six[] = {1, 1, 1, 1, 1, 0, 1};   // seis
int seven[] = {0, 0, 0, 0, 1, 1, 1};   // siete
int eight[] = {1, 1, 1, 1, 1, 1, 1}; // ocho
int nine[] = {1, 1, 0, 1, 1, 1, 1};   // nueve
int no_number[] = {1, 0, 0, 0, 0, 0, 0};   // numero mayor a nueve

// Se define el tipo de sensor a utilizar
#define DHTTYPE DHT11  
DHT dht(DHT_PIN, DHTTYPE);

//Variables sensor de movimiento
int pinStateCurrent   = LOW;  // Estado actual de la lectura del pin
int pinStatePrevious  = LOW;  // Estado anterior de la lectura del pin

//Variables
int isFlame = HIGH;             // Sensor flama. HIGH significa NO FLAME
bool flama;
bool movimiento;
float duration_us, distance_cm; //Sensor distancia
float h;                        //Sensor DHT
float t;                        //Sensor DHT
float f;                        //Sensor DHT
String numero;                  //Numero de Firebase /readings/numero
int num;

// Credenciales para conectarse a la red bajo protocolo WPA2-Personal
const char* ssid = "LAPTOP-Luis";
const char* password = "lf0602hf";

// Credenciales para el escritura de datos en Firebase
#define USER_EMAIL "a01735939@tec.mx"
#define USER_PASSWORD "a01735939"

// API Key de la DB en Firebase
#define API_KEY "AIzaSyC6y5rPcMl6RU7kMSaSRhd4m6ccrF0siNs"//AIzaSyAjjTHMIV0y394tayvijhU-aVVcKdkIZxU

// Definimos el URL de la base de datos
#define DATABASE_URL "https://pipin-ca62a-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;
// Valor booleano para evaluar el inicio de sesion exitoso
bool signupOK = false;

void setup() {
  Serial.begin(115200);       // Se incializa el puerto serial con tasa de informacion en 115200 bits/segundos
  dht.begin();                // Se inicializa la lectura del sensor DHT11
  delay(10);                  // Delay

  // Se configura TRING_PIN como output
  pinMode(TRIG_PIN, OUTPUT);
  // Se configura TRING_PIN como input
  pinMode(ECHO_PIN, INPUT);

  // Se inicializan los pines del display como salida 
  for (int i = 0; i<7; i++) pinMode(LEDs[i], OUTPUT);

  Serial.println();
  Serial.print("Connecting to ");
  // Se imprime el ssid de la red declarada
  Serial.println(ssid);
  // Se inicializa la conexion a Internet con las credenciales declaradas
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // Se asigna la variable API_KEY a config.api_key
  config.api_key = API_KEY;
  // Se asigna la variable DATABASE_URL a config.database_url
  config.database_url = DATABASE_URL;
  // Se verifica que el loggeo con la DB haya sido exitoso
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  // Se inicializa Firebase con las credenciales declaradas anteriormente
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

int value = 0;

// Funcion que ilumina los segmentos correspondientes del display 7 segmentos
// según el numero en parametro
void segment_display(unsigned char valor){
    switch(valor){
        case 0:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], zero[i]); break;
        case 1:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], one[i]); break;
        case 2:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], two[i]); break;
        case 3:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], three[i]); break;
        case 4:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], four[i]); break;
        case 5:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], five[i]); break;
        case 6:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], six[i]); break;
        case 7:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], seven[i]); break;
        case 8:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], eight[i]); break;
        case 9:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], nine[i]); break; 
        default:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], no_number[i]); break;          
    }
}

void sensorTempHum(){
  // Se lee humedad
  h = dht.readHumidity();
  // Se lee temperatura en grados Celsius, magnitud predeterminada
  t = dht.readTemperature();
  // Se lee temperatura en grados Fahrenheit, parametro true
  f = dht.readTemperature(true);
}

void sensorFlama(){
  isFlame = digitalRead(FLAMA_PIN);   // Se le asigna a isFlame el valor recibido por el sensor de flama
  if(isFlame==HIGH){
    flama=true;
  }
  else{
    flama=false;
  }
}

void sensorDistancia(){
  digitalWrite(TRIG_PIN, HIGH);
  // Genera un delay de 10 microsegundos para la medicion de la distancia
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Mide la duracion del pulso generado por el ECHO_PIN
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // Se calcula la distancia mediante una conversion
  distance_cm = 0.017 * duration_us;
}

void sensorMovimiento(){
  pinStatePrevious = pinStateCurrent;         // Se actualiza el estado anterior de la variable movimiento
  pinStateCurrent = digitalRead(S_MOV_PIN);   // Se actualiza el estado presente de la variable movimiento
  // En el caso de que la medicion anterior sea LOW y la actual HIGH, hay llama
  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {
    movimiento=true;
  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {
    movimiento=false;
  }
  // En el caso de que la medicion anterior sea HIGH y la actual LOW, no hay llama
}

void loop() {
  delay(2000);
  // Se llaman a la funciones dedicadas a las mediciones
  sensorTempHum();
  sensorFlama();
  sensorDistancia();
  sensorMovimiento();

  // Se validan diversas cuestiones para determinar que la conexion wifi y el logging in a la base de datos haya sido exitoso
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    // Para todas las escrituras de variables, se valida que la escritura haya sido exitosa
    // En caso de que no se pueda escribir la lectura, se imprime la razon del error
    // Se manda la variable t a la database en path readings/temperaturaC
    if (Firebase.RTDB.setFloat(&fbdo, "readings/temperaturaC", t)){
      Serial.println("PASSED");
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    // Se manda la variable f a la database en path readings/temperaturaF
    if (Firebase.RTDB.setFloat(&fbdo, "readings/temperaturaF", f)){
      Serial.println("PASSED");
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    // Se manda la variable f a la database en path readings/humedad
    if (Firebase.RTDB.setFloat(&fbdo, "readings/humedad", h)){
      Serial.println("PASSED");
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Se manda la variable flama a la database en path readings/isFlame
    if (Firebase.RTDB.setBool(&fbdo, "readings/isFlame", flama)){
      Serial.println("PASSED");
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Se manda la variable movimiento a la database en path readings/movement
    if (Firebase.RTDB.setBool(&fbdo, "readings/movement", movimiento)){
      Serial.println("PASSED");
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // Se manda la variable distance_cm a la database en path readings/distance
    if (Firebase.RTDB.setFloat(&fbdo, "readings/distance", distance_cm)){
      Serial.println("PASSED");
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    // Se valida el que haya una variable en la direccion especificada
    if (Firebase.RTDB.getString(&fbdo, "/readings/numero")){
      // Se lee la variable en /readings/numero
      numero = fbdo.stringData();
      num=numero.toInt();
    }
    segment_display(num);
  }
}
