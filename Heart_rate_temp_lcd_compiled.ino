#include <LiquidCrystal.h>
int val;
int tempPin = 1;
int sensor_pin = 0;                
int led_pin = 13;    

volatile int heart_rate;          
volatile int analog_data;              
volatile int time_between_beats = 600;            
volatile boolean pulse_signal = false;    
volatile int beat[10];         //heartbeat values will be sorted in this array    
volatile int peak_value = 512;          
volatile int trough_value = 512;        
volatile int thresh = 525;              
volatile int amplitude = 100;                 
volatile boolean first_heartpulse = true;      
volatile boolean second_heartpulse = false;    
volatile unsigned long samplecounter = 0;   //This counter will tell us the pulse timing
volatile unsigned long lastBeatTime = 0;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
 pinMode(led_pin,OUTPUT);        
  Serial.begin(115200);           
  interruptSetup();
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
Serial.print("BPM: ");
Serial.println(heart_rate);
delay(200);
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
lcd.print("BPM");
lcd.setCursor(0,1);
lcd.print(heart_rate);
delay(500);
lcd.clear();
  lcd.setCursor(0,0);
lcd.print("ALL IS WELL...");
lcd.setCursor(0,1);
lcd.print("Keep Smiling..");
delay(200);

  }

}

void interruptSetup()

{    

  TCCR2A = 0x02;  // This will disable the PWM on pin 3 and 11

  OCR2A = 0X7C;   // This will set the top of count to 124 for the 500Hz sample rate

  TCCR2B = 0x06;  // DON'T FORCE COMPARE, 256 PRESCALER

  TIMSK2 = 0x02;  // This will enable interrupt on match between OCR2A and Timer

  sei();          // This will make sure that the global interrupts are enable

}


ISR(TIMER2_COMPA_vect)

{ 

  cli();                                     

  analog_data = analogRead(sensor_pin);            

  samplecounter += 2;                        

  int N = samplecounter - lastBeatTime;      


  if(analog_data < thresh && N > (time_between_beats/5)*3)

    {     

      if (analog_data < trough_value)

      {                       

        trough_value = analog_data;

      }

    }


  if(analog_data > thresh && analog_data > peak_value)

    {        

      peak_value = analog_data;

    }                          



   if (N > 250)

  {                            

    if ( (analog_data > thresh) && (pulse_signal == false) && (N > (time_between_beats/5)*3) )

      {       

        pulse_signal = true;          

        digitalWrite(led_pin,HIGH);

        time_between_beats = samplecounter - lastBeatTime;

        lastBeatTime = samplecounter;     



       if(second_heartpulse)

        {                        

          second_heartpulse = false;   

          for(int i=0; i<=9; i++)    

          {            

            beat[i] = time_between_beats; //Filling the array with the heart beat values                    

          }

        }


        if(first_heartpulse)

        {                        

          first_heartpulse = false;

          second_heartpulse = true;

          sei();            

          return;           

        }  


      word runningTotal = 0;  


      for(int i=0; i<=8; i++)

        {               

          beat[i] = beat[i+1];

          runningTotal += beat[i];

        }


      beat[9] = time_between_beats;             

      runningTotal += beat[9];   

      runningTotal /= 10;        

      heart_rate = 60000/runningTotal;

    }                      

  }




  if (analog_data < thresh && pulse_signal == true)

    {  

      digitalWrite(led_pin,LOW); 

      pulse_signal = false;             

      amplitude = peak_value - trough_value;

      thresh = amplitude/2 + trough_value; 

      peak_value = thresh;           

      trough_value = thresh;

    }


  if (N > 2500)

    {                          

      thresh = 512;                     

      peak_value = 512;                 

      trough_value = 512;               

      lastBeatTime = samplecounter;     

      first_heartpulse = true;                 

      second_heartpulse = false;               

    }


  sei();                                

}
