int sensor_pin = A0; 
int output_value ;
int pumpPin = 2; //relay pin
int knapPin = 3;
int lampeRedPin = 4;
int lampeGreenPin =5;

void setup() {
  Serial.begin(9600);
  Serial.println("Reading From the Sensor ...");
  delay(2000);
   pinMode(pumpPin, OUTPUT);
   pinMode(knapPin, INPUT);
   pinMode(lampeRedPin, OUTPUT);
   pinMode(lampeGreenPin, OUTPUT);
  }

void loop() {
while(true) {
   output_value= analogRead(sensor_pin);
  int knap = digitalRead(knapPin);
  output_value = map(output_value,0,1023,0,100);
  Serial.print("Mositure : ");
  Serial.print(output_value);
  Serial.print("%, knap = ");
  Serial.println(knap);

  // Tjek om der er vand i beholderen
  if(digitalRead(knapPin)==HIGH) {
    digitalWrite(lampeGreenPin,LOW);
    digitalWrite(lampeRedPin,HIGH);
   continue;
  } else {
    digitalWrite(lampeRedPin,LOW);
    digitalWrite(lampeGreenPin,HIGH);
  }
  //delay(100);

  // Check om der skal vandes
  if(analogRead(output_value) >= 512.5 && digitalRead(knapPin)==LOW) {
    digitalWrite(pumpPin, HIGH);
    // Tid den vander i (ms)
    delay(100);
    digitalWrite(pumpPin, LOW);
   // delay(69999);
  }
  //delay(100); // 1hour
  }
}

  


