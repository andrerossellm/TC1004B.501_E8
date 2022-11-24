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

// Se define el tipo de sensor a utilizar
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
int numero;                     //Numero de Firebase /readings/numero

//Credenciales para inicio de sesion en red con protocolo WPA2-Enterprise
const char* ssid = "Tec";                 //Nombre de la red
#define EAP_IDENTITY "a01735939@tec.mx"   //Nombre de usuario para WPA2-Enterprise
#define EAP_PASSWORD "LF0602HF@tec2021"   //Contraseña de usuario para WPA2-Enterprise

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
  Serial.println(ssid);

  // WPA2-Enterprise
  WiFi.disconnect(true);    //
  WiFi.mode(WIFI_STA);   //init wifi mode
  //esp_wifi_set_mac(ESP_IF_WIFI_STA, &masterCustomMac[0]);
  Serial.print("MAC >> ");
  Serial.println(WiFi.macAddress());
  Serial.printf("Connecting to WiFi: %s ", ssid);
  //esp_wifi_sta_wpa2_ent_set_ca_cert((uint8_t *)incommon_ca, strlen(incommon_ca) + 1);
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  //esp_wpa2_config_t configW = WPA2_CONFIG_INIT_DEFAULT();
  //esp_wifi_sta_wpa2_ent_enable(&configW);
  esp_wifi_sta_wpa2_ent_enable();
  // WPA2 enterprise magic ends here
  WiFi.begin(ssid);

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
}

// Funcion para el sensor de movimiento
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

    if (Firebase.RTDB.getInt(&fbdo, "/readings/numero")){
      numero = fbdo.intData();
    }
    
    segment_display(numero);

  }
}
