#include <Servo.h>
#include <arduino-timer.h>

#define AZ_PIN 9
#define AL_PIN 10

// Function prototypes
int average(int val_1, int val_2);

Servo AzimuthServo;
Servo AltitudeServo;

auto timer = timer_create_default();

int g_val;    // variable to read the value from the analog pin
unsigned long g_systick = 0;
const long g_interval = 15;
const long beat = 1000;
int g_az_val = 90;
int g_alt_val =  90;

// heartbeat ISR
volatile bool heartbeat(void *){
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

  // // Servo test
  // int pos = 0;
  //   for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   AzimuthServo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   AzimuthServo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }
  //  int pos =90;
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   AltitudeServo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   AltitudeServo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }

}

void loop() {
  
  int photoResQ1 = analogRead(A0);
  int photoResQ2 = analogRead(A1);
  int photoResQ3 = analogRead(A2);
  int photoResQ4 = analogRead(A3);

  // azimuth pair
  if(average(photoResQ2,photoResQ3) > average(photoResQ1,photoResQ4)){
    g_az_val++;
    AzimuthServo.write(g_az_val); 
    delay(5);
  }
  else if (average(photoResQ2,photoResQ3) < average(photoResQ1,photoResQ4)){
    g_az_val--;
    AzimuthServo.write(g_az_val); 
    delay(5);
  }
  else
  {
    g_az_val = g_az_val;
    delay(5);
  }

  // altitude pair
  if(average(photoResQ1,photoResQ2) > average(photoResQ3,photoResQ4)){
    g_alt_val++;
    AltitudeServo.write(g_alt_val);
    delay(5);
  }
  else if (average(photoResQ1,photoResQ2) < average(photoResQ3,photoResQ4)){
    g_alt_val--;
    AltitudeServo.write(g_alt_val);
    delay(5);
  }
  else
  {
    g_alt_val = g_alt_val;
  }


  // Serial.print("Q1: ");
  // Serial.println(photoResQ1);
  // Serial.print("Q2: ");
  // Serial.println(photoResQ2);
  // Serial.print("Q3: ");
  // Serial.println(photoResQ3);
  // Serial.print("Q4: ");
  // Serial.println(photoResQ4);
  // Serial.print("Q2 Q3 Avg: ");
  // Serial.println(average(photoResQ2,photoResQ3));
  // Serial.print("Q1 Q4 Avg: ");
  // Serial.println(average(photoResQ1,photoResQ4));
  // Serial.print("Q1 Q2 Avg: ");
  // Serial.println(average(photoResQ1,photoResQ2));
  // Serial.print("Q3 Q4 Avg: ");
  // Serial.println(average(photoResQ3,photoResQ4));

  unsigned long currentMillis = millis();

  // // non-blocking delay to move servo
  // if (currentMillis - g_systick >= g_interval) {
  //   g_systick = currentMillis;
  //   g_val = analogRead(POTPIN);            // reads the value of the potentiometer (value between 0 and 1023)
  //   g_val = map(g_val, 0, 1023, 0, 180);     // scale it for use with the servo (value between 0 and 180)
  //   AzimuthServo.write(g_val);                  // sets the servo position according to the scaled value
  // }
  delay(200);
  timer.tick();
}