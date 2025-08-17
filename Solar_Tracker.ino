#include <Servo.h>
#include <arduino-timer.h>

#define AZ_PIN 9
#define AL_PIN 8

Servo AzimuthServo;
Servo AltitudeServo;

Timer<1> SystemTimer;

int g_val;    // variable to read the value from the analog pin
unsigned long g_systick = 0;
const long g_interval = 15;
const long beat = 1000;
int g_az_val = 90;
int g_alt_val =  90;

// heartbeat ISR
bool heartbeat(void *){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle the LED
  return true; // repeat? true
}

void setup() {
  AzimuthServo.attach(AZ_PIN);  // attaches the servo on pin 9 to the Servo object
  AltitudeServo.attach(AL_PIN);
  SystemTimer.every(beat, heartbeat); // schedule timer to tick every 1000ms
}

void loop() {
  
  int photoResQ1 = analogRead(A0);
  int photoResQ2 = analogRead(A2);
  int photoResQ3 = analogRead(A3);
  int photoResQ4 = analogRead(A4);

  // Q1-Q2 pair
  if( photoResQ1 > photoResQ2){
    g_az_val++;
    AzimuthServo.write(g_az_val); 
  }
  else if (photoResQ1 < photoResQ2){
    g_az_val--;
    AzimuthServo.write(g_az_val); 
  }
  else
  {
    g_az_val = g_az_val;
  }

  // Q3-Q4 pair
  if( photoResQ3 > photoResQ4){
    g_az_val++;
    AzimuthServo.write(g_az_val); 
  }
  else if (photoResQ3 < photoResQ4){
    g_az_val--;
    AzimuthServo.write(g_az_val); 
  }
  else
  {
    g_az_val = g_az_val;
  }

  // Q2-Q3
  if( photoResQ2 > photoResQ3){
    g_alt_val++;
    AltitudeServo.write(g_alt_val);
  }
  else if (photoResQ2 < photoResQ3){
    g_alt_val--;
    AltitudeServo.write(g_alt_val);
  }
  else
  {
    g_alt_val = g_alt_val;
  }

  // Q1-Q4
  if( photoResQ1 > photoResQ4){
    g_alt_val++;
    AltitudeServo.write(g_alt_val);
  }
  else if (photoResQ1 < photoResQ4){
    g_alt_val--;
    AltitudeServo.write(g_alt_val);
  }
  else
  {
    g_alt_val = g_alt_val;
  }

  unsigned long currentMillis = millis();

  // // non-blocking delay to move servo
  // if (currentMillis - g_systick >= g_interval) {
  //   g_systick = currentMillis;
  //   g_val = analogRead(POTPIN);            // reads the value of the potentiometer (value between 0 and 1023)
  //   g_val = map(g_val, 0, 1023, 0, 180);     // scale it for use with the servo (value between 0 and 180)
  //   AzimuthServo.write(g_val);                  // sets the servo position according to the scaled value
  // }
  
  SystemTimer.tick();
}