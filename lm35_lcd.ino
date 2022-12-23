#include <LiquidCrystal.h>
int val;
int tempPin = 1;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup()
{
pinMode(6,OUTPUT);
lcd.begin(16, 2);
Serial.begin(9600);
lcd.setCursor(0,0);
lcd.print("Welcome");
lcd.setCursor(0,1);
lcd.print("Loading....");
delay(3000);
}
void loop()
{
val = analogRead(tempPin);
float mv = ( val/1024.0)*5000;
float cel = mv/10;
float farh = (cel*9)/5 + 32;

Serial.print("TEMPRATURE = ");
Serial.print(cel);
Serial.print("*C");
Serial.println();
delay(1000);

Serial.print("TEMPRATURE = ");
Serial.print(farh);
Serial.print("*F");
Serial.println();
delay(1000);

if (farh >= 99)
{
  lcd.clear();
  lcd.setCursor(0,0);
lcd.print("Anxiety Detected");
lcd.setCursor(0,1);
lcd.print("Healing....");
digitalWrite(6,HIGH),
delay(100);
digitalWrite(6,LOW),
delay(100);
  
  }
else
{
  digitalWrite(6,LOW),
  lcd.clear();
  lcd.setCursor(0,0);
lcd.print("Body Temp...");
lcd.setCursor(0,1);
lcd.print(farh);
delay(500);
lcd.clear();
  lcd.setCursor(0,0);
lcd.print("ALL IS WELL...");
lcd.setCursor(0,1);
lcd.print("Keep Smiling..");
delay(200);

  }

}
