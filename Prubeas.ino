#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define TRIG_PIN 23     // ESP32 pin GIOP23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 22     // ESP32 pin GIOP22 connected to Ultrasonic Sensor's ECHO pin
#define FLAMA_PIN 17   // Pin input para el puerto DO . Sensor flama
#define S_MOV_PIN 19    // Pin input para el sensor de movimiento
#define DHT_PIN 4     // Pin input para el sensor DHT

//Variables sensor de movimiento
int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin

//DHT11
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHT_PIN, DHTTYPE);

//Variables
int isFlame = HIGH;             // Sensor flama. HIGH MEANS NO FLAME
float duration_us, distance_cm; //Sentor distancia
float h;                        //Sensor DHT
float t;                        //Sensor DHT
float f;                        //Sensor DHT

void setup() {
  // Se inicializa el puerto serial con 9600
  Serial.begin (9600);
  Serial.println(F("DHTxx test!"));
  
  // Se inicializa el sensor dht
  dht.begin();

  // Se declara TRIG_PIN con modo Output
  pinMode(TRIG_PIN, OUTPUT);
  // Se declara ECHO_PIN con modo Input
  pinMode(ECHO_PIN, INPUT);
  //Se declara S_MOV_PIN con modo Input
  pinMode(S_MOV_PIN, INPUT);
  //Se declara FLAMA_PIN con modo Input
  pinMode(FLAMA_PIN, INPUT);
}

void sensorFlama(){
  isFlame = digitalRead(FLAMA_PIN);//Readd the data gien by the flame sensor
  if (isFlame== HIGH)  //if it is low
  {
    Serial.println("FLAME, FLAME, FLAME"); //Print Flame Flame
    digitalWrite(LED_BUILTIN, HIGH);  //LED on
  }
  else if(isFlame== LOW){                               
    Serial.println("No flame"); //print no  flame
    digitalWrite(LED_BUILTIN, LOW);    //off the LED
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

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
}

void sensorMovimiento(){
  pinStatePrevious = pinStateCurrent;         // Se actualiza el estado anterior de la variable movimiento
  pinStateCurrent = digitalRead(S_MOV_PIN);   // Se actualiza el estado presente de la variable movimiento
  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {
    // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    // TODO: turn on alarm, light or activate a device ... here
  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {
    // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    // TODO: turn off alarm, light or deactivate a device ... here
  }
}

void sensorTempHum(){
  // Se lee humedad
  h = dht.readHumidity();
  // Se lee temperatura en grados Celsius, magnitud predeterminada
  t = dht.readTemperature();
  // Se lee temperatura en grados Fahrenheit, parametro true
  f = dht.readTemperature(true);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C , "));
  Serial.print(f);
  Serial.print(F("°F\n"));
}

void loop() {
  delay(2000);
  sensorMovimiento();
  sensorDistancia();
  sensorTempHum();
  sensorFlama();
  Serial.print(F("\n"));
}
