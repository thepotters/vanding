/*
int fugtsensor1 = A0;
int relay1 = 7;
int fugtreading = analogRead(fugtsensor1);
unsigned long previousmillis = 0;
const long vandingstid  = 10000;
*/

int maxRelays = 4;
int fugtSensor[4] = {0, 1, 2, 3};
int relay[4]= {7, 3, 9, 11};
int fugtReading[4];
unsigned long previousMillis = 0;
const long vandingsTid  = 10000;

void setup() 
{
    for (int i = 0; i < maxRelays; i++) 
  {
    pinMode(fugtSensor[i], INPUT);
    pinMode(relay[i], OUTPUT);
  }

  

  Serial.begin(9600);
  Serial.println("Reading Sensors....");
}


void loop() 
{
  Serial.println("Debug:");
  for (int i = 0; i < maxRelays; i++) 
  {
    fugtReading[i] = analogRead(fugtSensor[i]);
     Serial.println("Fugtighed:");
     Serial.print(analogRead(fugtSensor[i]));
  }
 
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= 1000) 
  {
   previousMillis = currentMillis;
   for (int i = 0; i < maxRelays; i++) {
     if (fugtReading[i] <= 700) 
      {
       digitalWrite(relay[i], HIGH);
       if (currentMillis - previousMillis >= vandingsTid)
        {
         digitalWrite(relay[i], LOW);
        }
      }
    if (digitalRead(relay[i] == HIGH))
     {
        Serial.println("Relay on #" + relay[i]);
      //  Serial.print(relay[i];
     //   Serial.print(" Tændt");
     }
   }
  }

}
