#include <Servo.h>
#include <arduino-timer.h>
#include <SPI.h>
#include <SD.h>

#define AZ_PIN 9
#define AL_PIN 10

// Function prototypes
int average(int val_1, int val_2);
void logData(float dataToWrite);
int servoWrite(int step, char motor);

Servo AzimuthServo;
Servo AltitudeServo;

auto timer = timer_create_default();

const int chipSelect = 4;
int g_val;    // variable to read the value from the analog pin
unsigned long g_systick = 0;
const long beat = 1000;
int g_az_val = 90;
int g_alt_val =  90;

// heartbeat ISR
bool heartbeat(void *){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle the LED
  return true; // repeat? true
}

void setup() {
  delay(3000);
  Serial.begin(115200);
  AzimuthServo.attach(AZ_PIN);  // attaches the servo on pin 9 to the Servo object
  AltitudeServo.attach(AL_PIN);
  pinMode(LED_BUILTIN, OUTPUT); 
  timer.every(1000, heartbeat); // schedule timer to tick every 1000ms

  // see if the card is present and can be initialised
  /*if(!SD.begin(chipSelect)){
    Serial.println("Card failed, or not present.");
    while(1);
  }
  Serial.println("Card intialised.");*/
}

void loop() {
  
  int photoResQ1 = analogRead(A0);
  int photoResQ2 = analogRead(A1);
  int photoResQ3 = analogRead(A2);
  int photoResQ4 = analogRead(A3);

  // azimuth pair
    if(average(photoResQ2,photoResQ3) > average(photoResQ1,photoResQ4)){
      g_az_val++;
      char az = 'z';
      servoWrite(g_az_val, az); 
    }
    else if (average(photoResQ2,photoResQ3) < average(photoResQ1,photoResQ4)){
      g_az_val--;
      char az = 'z';
      servoWrite(g_az_val, az); 
    }
    else
    {
      g_az_val = g_az_val;
    }

    // altitude pair
    if(average(photoResQ1,photoResQ2) > average(photoResQ3,photoResQ4)){
      g_alt_val++;
      char al = 'l';
      servoWrite(g_az_val, al); 
    }
    else if (average(photoResQ1,photoResQ2) < average(photoResQ3,photoResQ4)){
      g_alt_val--;
      char al = 'l';
      servoWrite(g_az_val, al); 
    }
    else
    {
      g_alt_val = g_alt_val;
    }
  // // non-blocking delay to move servo
  // if (currentMillis - g_systick >= g_interval) {
  //   g_systick = currentMillis;
  //   g_val = analogRead(POTPIN);            // reads the value of the potentiometer (value between 0 and 1023)
  //   g_val = map(g_val, 0, 1023, 0, 180);     // scale it for use with the servo (value between 0 and 180)
  //   AzimuthServo.write(g_val);                  // sets the servo position according to the scaled value
  // }
  timer.tick();
}