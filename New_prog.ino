  #include <dht.h>
  dht DHT;
  #define DHT11_PIN 2
  
  int maxRelays = 5;
  int fugtSensor[5] = {A0, A1, A2, A3, A4};
  int val [5];
  int lightSensor = A8;
  int relay[5]= {22, 23, 24, 25, 26};
  unsigned long previousMillis = 0;
  unsigned long startedWater[5];
  const long vandingsTid  = 3000;
  unsigned long lastInform;
  unsigned long tempLastRead;
  int nextInform = 0;
  unsigned long timestamp;
  float luft;
  float temp;

//-------------------------------------------------------------------------------------------------
// OLED 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

//--------------------------------------------------------------------------------


void setup() {
  for (int i = 0; i < maxRelays; i++) {
    pinMode(fugtSensor[i], INPUT);
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], HIGH);
  }
    pinMode(lightSensor, INPUT);
  
    lastInform = millis();
    Serial.begin(9600);
    Serial.println("Reading Sensors....");  

    //----------- OLDE ---------------
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
     Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
  // Invert and restore display, pausing in-between
  
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
  Serial.println("Entering loop");
   
}




void loop() {
  timestamp = millis();
  for (int i = 0; i < maxRelays; i++) {
    if (analogRead(fugtSensor[i]) >= 500 && digitalRead(relay[i]) == HIGH && timestamp - 10000 > startedWater[i] ) {
      Serial.print("Started relay #");
      Serial.println(i);
      Serial.print("Wetness = ");
      Serial.println(analogRead(fugtSensor[i]));
      digitalWrite(relay[i], LOW);
      startedWater[i] = timestamp;
    }
    if (digitalRead(relay[i]) == LOW && timestamp - startedWater[i] >= vandingsTid) {
      Serial.print("Stopped relay #");
      Serial.println(i);
      digitalWrite(relay[i], HIGH);
    }
  }

   if (timestamp - 3000 >= tempLastRead) {
    int chk = DHT.read11(DHT11_PIN);
    temp = DHT.temperature;
    luft = DHT.humidity;
    Serial.println(temp);
    Serial.println(luft);
    for (int i = 0; i < maxRelays; i++) {
      val[i] = analogRead(fugtSensor[i]);  
      val[i] = map(val[i], 1024, 250, 0, 100); 
    }
    

    tempLastRead = timestamp;
   }

   
  OLED();
//  inform();

}

// --------- OLED
void OLED() {
  if (timestamp > lastInform) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20,0);
    display.print("Potteplante #");
    display.print(nextInform + 1);
    display.setCursor(5,10);
    display.print("Humidity: ");
    display.print(val[nextInform]);
 //   display.print(analogRead(fugtSensor[nextInform]));
    display.print(" %");
 //   display.setCursor(5, 20);
 //   display.print("Lys: ");
 //   display.print(analogRead(lightSensor));
    display.setCursor(5,20);
    display.print("Temp: ");
    display.print(DHT.temperature);
    display.print(" C");
    display.setCursor(5,30);
    display.print("Luft: ");
    display.print(DHT.humidity);
    display.print(" %");
    display.setCursor(5,40);
    display.print("Vandet: ");
    display.print(timeToString((timestamp - startedWater[nextInform]) / 1000));
    display.display();
   
    
    
    lastInform = timestamp + 2000;
    nextInform++;
    
    if (nextInform >= maxRelays) {
      nextInform = 0;

    
  }
 }
}




String timeToString(unsigned long input) {
  String formattedTime = "";
  int timeRemoved = input / 86400;
  // Tilføj dage
  if (timeRemoved != 0) {
    input -= timeRemoved * 86400;
    formattedTime = formattedTime + timeRemoved + "D ";
  }
  // Tilføj timer
  timeRemoved = input / 3600;
  if (timeRemoved != 0) {
    input -= timeRemoved * 3600;
    formattedTime = formattedTime + timeRemoved + "H ";
  }
  // Tilføj minutter
  timeRemoved = input / 60;
  if (timeRemoved != 0) {
    input -= timeRemoved * 60;
    formattedTime = formattedTime + timeRemoved + "M ";
  }
  // Tilføj sekunder
  if (input != 0) {
    formattedTime = formattedTime + input + "S ";
  }
  return formattedTime;
}





/*
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
   // Serial.println(DHT.temperature);
    Serial.print("Luftfugtighed: ");
   // Serial.println(DHT.humidity);
    Serial.println();
    lastInform = millis() + 2000;
    nextInform++;
    if (nextInform >= maxRelays) {
      nextInform = 0;
    }
  }
}







/*
void wifi() {


Read data
 - humidity, 
 - light, 
 - temp, 
 - fugtsensor
 - relays.
 
Turn off / Turn on
 - Relay
 
Last cycle
 - sidste vanding 
 - logging   
 
 if(wifiSerial.available()>0){ 
   String message = readWifiSerialMessage(); 
   if(find(message,"esp8266:")){ 
      String result = sendToWifi(message.substring(8,message.length()),responseTime,DEBUG); 
     if(find(result,"OK")) 
       sendData("\n"+result); 
     else 
       sendData("\nErrRead");               //At command ERROR CODE for Failed Executing statement 
   }else 
   if(find(message,"HELLO")){  //receives HELLO from wifi 
       sendData("\\nHI!");    //arduino says HI 
   }else if(find(message,"LEDON")){ 
     digitalWrite(13,HIGH); 
   }else if(find(message,"LEDOFF")){ 
     digitalWrite(13,LOW); 
   } 
   else{ 
     sendData("\nErrRead");                 //Command ERROR CODE for UNABLE TO READ 
   } 
   
 }  
}
 */
