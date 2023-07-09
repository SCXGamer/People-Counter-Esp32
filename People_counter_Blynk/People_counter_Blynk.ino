#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define TRIGGER_PIN_1 0
#define ECHO_PIN_1 2

#define TRIGGER_PIN_2 4
#define ECHO_PIN_2 16

char auth[] = "*********************";
char ssid[] = "*************";
char pass[] = "*************";

BlynkTimer timer;

int peopleCount = 0;

void sendUltrasonicData()
{
  digitalWrite(TRIGGER_PIN_1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_1, LOW);

  long duration_1 = pulseIn(ECHO_PIN_1, HIGH);
  int distance_1 = duration_1 * 0.034 / 2;

  digitalWrite(TRIGGER_PIN_2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_2, LOW);

  long duration_2 = pulseIn(ECHO_PIN_2, HIGH);
  int distance_2 = duration_2 * 0.034 / 2;

  Serial.print("Distance Sensor 1: ");
  Serial.print(distance_1);
  Serial.print(" cm | Distance Sensor 2: ");
  Serial.print(distance_2);
  Serial.println(" cm");

  if (distance_1 < distance_2) {
    peopleCount++;
    Serial.println("Person entered the room");
  }
  else if (distance_2 < distance_1) {
    if (peopleCount > 0) {
      peopleCount--;
      Serial.println("Person exited the room");
    }
    else {
      Serial.println("No one to exit the room");
    }
  }

  Serial.print("People count in the room: ");
  Serial.println(peopleCount);
  
  Blynk.virtualWrite(V0, peopleCount); // Send people count to Blynk Value Display widget
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  
  pinMode(TRIGGER_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  
  pinMode(TRIGGER_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  
  timer.setInterval(1000, sendUltrasonicData);
}

void loop()
{
  Blynk.run();
  timer.run();
}
