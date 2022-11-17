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

// DHT 11
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

//Variables sensor de movimiento
int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin

//Variables
int isFlame = HIGH;             // Sensor flama. HIGH MEANS NO FLAME
bool flama;
bool movimiento;
float duration_us, distance_cm; //Sensor distancia
float h;                        //Sensor DHT
float t;                        //Sensor DHT
float f;                        //Sensor DHT
int randomNumber;               //Numero aleatorio

const char* ssid = "MEGACABLE-C29F";
const char* password = "P6Jx2ncJ";

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

void sensorTempHum(){
  // Se lee humedad
  h = dht.readHumidity();
  // Se lee temperatura en grados Celsius, magnitud predeterminada
  t = dht.readTemperature();
  // Se lee temperatura en grados Fahrenheit, parametro true
  f = dht.readTemperature(true);
}

void sensorFlama(){
  isFlame = digitalRead(FLAMA_PIN);//Readd the data gien by the flame sensor
  if(isFlame==HIGH){
    flama=true;
  }
  else{
    flama=false;
  }
}

void sensorDistancia(){
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;
  Serial.print(distance_cm);
  Serial.print("\n");
}

void sensorMovimiento(){
  pinStatePrevious = pinStateCurrent;         // Se actualiza el estado anterior de la variable movimiento
  pinStateCurrent = digitalRead(S_MOV_PIN);   // Se actualiza el estado presente de la variable movimiento
  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {
    movimiento=true;
  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {
    movimiento=false;
  }
}

void loop() {
  delay(2000);
  //Se llama a la funcion que lee temp y humedad
  sensorTempHum();
  sensorFlama();
  sensorDistancia();
  sensorMovimiento();

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    randomNumber=random(9);
    // Se manda la variable t a la database en path readings/temperaturaC
    if (Firebase.RTDB.setFloat(&fbdo, "readings/temperaturaC", t)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    // Se manda la variable f a la database en path readings/temperaturaF
    if (Firebase.RTDB.setFloat(&fbdo, "readings/temperaturaF", f)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    // Se manda la variable f a la database en path readings/humedad
    if (Firebase.RTDB.setFloat(&fbdo, "readings/humedad", h)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Se manda la variable flama a la database en path readings/isFlame
    if (Firebase.RTDB.setBool(&fbdo, "readings/isFlame", flama)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Se manda la variable movimiento a la database en path readings/movement
    if (Firebase.RTDB.setBool(&fbdo, "readings/movement", movimiento)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // Se manda la variable distance_cm a la database en path readings/distance
    if (Firebase.RTDB.setFloat(&fbdo, "readings/distance", distance_cm)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    // Se manda la variable randomNumber a la database en path readings/numero
    if (Firebase.RTDB.setInt(&fbdo, "readings/numero", randomNumber)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}
