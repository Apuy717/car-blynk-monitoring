
#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

char auth[] = "Mr-XJ1dgfuKJLINypD0mZYUUMpJP4qrj";
char ssid[] = "******";
char pass[] = "******";

double speed = 0;
double jarakTempuh = 0;
double persentaseJarak = 0;
unsigned long myTime;
unsigned long second = 0;
unsigned long currentTIme;

unsigned long lastflash;
uint8_t GPIO_Pin = 12;
int RPM;
void IRAM_ATTR sens()
{
  RPM++;
}

void getSpeedMeterSecond(int rpm)
{
  float roda = 6.5;
  float diameterRoda = roda / 100;
  float jariJari = roda / 2;
  float kelipatan = 22 / jariJari;
  int detik = 60;
  speed = (rpm * 2 * kelipatan * diameterRoda) / detik;
  Blynk.virtualWrite(V1, speed);
}

void getJarakTempuh()
{
  jarakTempuh = speed * second;
  persentaseJarak = jarakTempuh / 100;
  Blynk.virtualWrite(V2, jarakTempuh);
  Blynk.virtualWrite(V4, persentaseJarak);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  // IR Infrared sensor
  // SENSOR: GPIO0 (NodeMCU - D6)
  pinMode(GPIO_Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GPIO_Pin), sens, RISING);
}

void loop()
{
  delay(1000);
  Blynk.run();
  noInterrupts();
  int wings = 1;
  int RPMnew = RPM / wings;
  getSpeedMeterSecond(RPMnew);
  currentTIme = millis();
  if (RPMnew >= 1)
  {
    myTime = millis() + currentTIme;
    second = myTime / 1000;
    Blynk.virtualWrite(V3, second);
    getJarakTempuh();
  }
  Serial.print(RPMnew);
  Serial.print(" Rot/sec :");
  Serial.print((RPMnew * 60));
  Serial.println("Rot/min. ");
  RPM = 0;
  interrupts();
}
