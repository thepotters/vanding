
int maxRelays = 4;
int fugtSensor[4] = {A0, A0, A0, A0};
int lightSensor = A1;
int temperature = A2;
int humidtity = A3;

int relay[4]= {7, 3, 9, 11};
unsigned long previousMillis = 0;
unsigned long startedWater[4];
const long vandingsTid  = 3000;

unsigned long lastInform;
int nextInform = 0;

void setup() {
  for (int i = 0; i < maxRelays; i++) {
    pinMode(fugtSensor[i], INPUT);
    pinMode(relay[i], OUTPUT);
  }
  pinMode(lightSensor, INPUT);
  pinMode(temperature, INPUT);
  pinMode(humidtity, INPUT);
  
  lastInform = millis();
  Serial.begin(9600);
  Serial.println("Reading Sensors....");
}


void loop() {
  for (int i = 0; i < maxRelays; i++) {
    if (analogRead(fugtSensor[i]) <= 700 && digitalRead(relay[i]) == LOW && millis() - 10000 > startedWater[i] ) {
      digitalWrite(relay[i], HIGH);
      startedWater[i] = millis();
    }
    if (digitalRead(relay[i]) == HIGH && millis() - startedWater[i] >= vandingsTid) {
      digitalWrite(relay[i], LOW);
    }
  }
  inform();
}

void inform() {
  if (millis() >= lastInform) {
    Serial.print("=== Potteplante #");
    Serial.print(nextInform + 1);
    Serial.println(" ===");
    Serial.print("Fugtighedsniveau: ");
    Serial.println(analogRead(fugtSensor[nextInform]));
    Serial.print("Lys niveau: ");
    Serial.println(analogRead(lightSensor));
    Serial.print("Temperatur: ");
    Serial.println(analogRead(temperature));
    Serial.print("Luftfugtighed: ");
    Serial.println(analogRead(humidtity));
    Serial.println();
    lastInform = millis() + 2000;
    nextInform++;
    if (nextInform >= maxRelays) {
      nextInform = 0;
    }
  }
}
