#include "config.h"
#include <ESP8266WiFi.h>
volatile unsigned long timerSensor1 = 0;
volatile unsigned long timerSensor2 = 0;
WiFiClient client;

ICACHE_RAM_ATTR void Sensor1() {
  detachInterrupt(digitalPinToInterrupt(gpioSensor1));
  timerSensor1 = millis();
}

ICACHE_RAM_ATTR void Sensor2() {
  detachInterrupt(digitalPinToInterrupt(gpioSensor2)); // Interrupt wird abgeschaltet, bis das Ereignis ausgewertet wurde damit weitere Ereignisse nicht das Ergebnis verfälschen
  timerSensor2 = millis();
}

void count() {
  int dir = 0;
  unsigned int Speed;
  if (timerSensor1 > timerSensor2) {
    dir = 1; // Richtung bestimmen, 1 falls in Richtung Sensor1 zu Sensor 2 bewegt wurde. 0 falls andersrum
    Speed = (sensorDistance * 36) / ((timerSensor1 - timerSensor2) * 10); // Geschwindigkeit berechnen
  } else {
    Speed = (sensorDistance * 36) / ((timerSensor2 - timerSensor1) * 10);
  }
    
  if (Speed < minSpeed || Speed > maxSpeed) return; // Plausibilitätsprüfung der Geschwindigkeit
  

  Serial.println("sensorID=" + String(sensorID) + 
                 " speed=" + String(Speed) +
                 " direction=" + String(dir));

  //Daten übermitteln:
  if (WiFi.status() == WL_CONNECTED) {
    if (client.connect(host, port)) {
      if (client.connected()) {
        client.print(String("GET ") + url + 
                      "sensorID=" + String(sensorID) + 
                      "&speed=" + String(Speed) +
                      "&direction=" + String(dir) +
                      " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Connection: close\r\n\r\n");
      }
    }
  }
}

void sensorReset() {
  timerSensor1 = 0;
  timerSensor2 = 0;
  attachInterrupt(digitalPinToInterrupt(gpioSensor1), Sensor1, RISING);
  attachInterrupt(digitalPinToInterrupt(gpioSensor2), Sensor2, RISING);
}


void setup() {
  Serial.begin(9600);
  Serial.println("");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(gpioSensor1, INPUT);
  pinMode(gpioSensor2, INPUT);
  attachInterrupt(digitalPinToInterrupt(gpioSensor1), Sensor1, RISING);
  attachInterrupt(digitalPinToInterrupt(gpioSensor2), Sensor2, RISING);
}

void loop() {
  if (timerSensor1 != 0 && timerSensor2 != 0) {
    count();
    while (millis() - timerSensor1 < downtime || millis() - timerSensor2 < downtime) {
      // Warteschleife für die Totzeit bis zur nächsten Messung
    }
    sensorReset();
  }
  if (timerSensor1 != 0 && millis() - timerSensor1 > timeout) sensorReset();
  if (timerSensor2 != 0 && millis() - timerSensor2 > timeout) sensorReset();
}
