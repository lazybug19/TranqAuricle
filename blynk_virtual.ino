//Made for arduino
//by ArduiCreate

#include <ESP8266WiFi.h>//Install these library
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

#define BLYNK_PRINT Serial

float vref = 3.3;
float resolution = vref / 1023.0;
float temperature;
float farh;

char auth[] = "te9jFKJgCNqKK4sqqtzMeWmtRuKIiHe7";//yourauthtoken
char ssid[] = "Realme 7";//name of your wifi
char pass[] = "d81fb8ec8435";//password of wifi

SimpleTimer timer;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000, sendUptime);
}

void sendUptime()
{
  float temperature = analogRead(A0);//lm35 is connected to pin A0 on NodeMcu
  temperature = (temperature * resolution);
  temperature = temperature * 100.0;
  farh = (temperature*9)/5 + 32;
  Serial.print("LM35 temperature: ");//serial print the value 
  Serial.println(farh);
  Blynk.virtualWrite(V0, farh );//send the value to blynk application
}

void loop()
{
  Blynk.run();
  timer.run();
}
//Thanks for your use
