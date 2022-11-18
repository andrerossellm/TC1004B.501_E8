//https://www.juanjobeunza.com/esp32-wifi/
//https://create.arduino.cc/projecthub/electropeak/connecting-arduino-to-firebase-to-send-receive-data-cd8805
#include <esp_wpa2.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>  
#include <addons/TokenHelper.h>
#include "Arduino.h"
#include "addons/RTDBHelper.h"
#include "DHT.h"

#define TRIG_PIN 23     // ESP32 pin GIOP23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 22     // ESP32 pin GIOP22 connected to Ultrasonic Sensor's ECHO pin
#define FLAMA_PIN 17   // Pin input para el puerto DO . Sensor flama
#define S_MOV_PIN 19    // Pin input para el sensor de movimiento
#define DHTPIN 4

//Se declaran los pines a usar para el display
int LEDs[] = {25,16,5,18,21,3,1};
//25 g, 16 f, 5 e, 18 d, 3 c, 21 b, 1 a

//se declaran los arreglos que forman los dígitos
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

//Variables
int numero;               //Numero de Firebase /readings/numero

//const char* ssid = "MEGACABLE-C29F";
//const char* password = "P6Jx2ncJ";
const char* ssid = "TP-LINK_343A";
const char* password = "24038614";

// Firebase insert auth
#define USER_EMAIL "a01735939@tec.mx"
#define USER_PASSWORD "a01735939"

// Insert Firebase project API Key
#define API_KEY "AIzaSyC6y5rPcMl6RU7kMSaSRhd4m6ccrF0siNs"//AIzaSyAjjTHMIV0y394tayvijhU-aVVcKdkIZxU

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://pipin-ca62a-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;

bool signupOK = false;

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx readings!"));
  dht.begin();
  delay(10);

  // Se inicializan los pines del display como salida 
  for (int i = 0; i<7; i++) pinMode(LEDs[i], OUTPUT);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Serial.print("MAC >> ");
  Serial.println(WiFi.macAddress());
  Serial.printf("Connecting to WiFi: %s ", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
///* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

int value = 0;

//función que despliega del 0 al F
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

void lectura_numero(){

}

void loop() {
  delay(2000);

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.getInt(&fbdo, "/readings/numero")){
        numero = fbdo.intData();
        Serial.println(numero);
    }
    
    segment_display(numero);
  }
}
