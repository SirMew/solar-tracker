#include <Servo.h>
#include <arduino-timer.h>
#include <SPI.h>
#include <SD.h>

#define AZ_PIN 9
#define AL_PIN 10
// Working tracker
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

struct PID_parameters{
  float K_p;
  float K_i;
  float K_d;
};

struct PID_parameters K_azimuth = {1,1,1};
struct PID_parameters K_altitude = {1,1,1};

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
  timer.every(500, heartbeat); // schedule timer to tick every 1000ms

  // see if the card is present and can be initialised
  /*if(!SD.begin(chipSelect)){
    Serial.println("Card failed, or not present.");
    while(1);
  }
  Serial.println("Card intialised.");*/
}

void loop() {
  
// Azimuth_reference = (average(photoResQ2,photoResQ3) - average(photoResQ1,photoResQ4)) == 0
// Altitude_reference = (average(photoResQ1,photoResQ2) - average(photoResQ3,photoResQ4)) == 0
  int photoResQ1 = analogRead(A0);
  int photoResQ2 = analogRead(A1);
  int photoResQ3 = analogRead(A2);
  int photoResQ4 = analogRead(A3);

  float setpoint_reference = 0.0;
  float azimuth_feedback = (average(photoResQ2,photoResQ3) - average(photoResQ1,photoResQ4));
  float altitude_feedback = (average(photoResQ1,photoResQ2) - average(photoResQ3,photoResQ4));
  float azimuth_error = setpoint_reference - azimuth_feedback;
  float altitude_error = setpoint_refernce - altitude_feedback;
  // TO DO: Add integral and derivative
  float control_variable_az = K_azimuth.K_p*azimuth_error + K_azimuth.K_i*(0) + K_azimuth.K_d*(0);
  float control_variable_alt = K_altitude.K_p*altitude_error + K_altitude.K_i*(0) + K_altitude.K_d*(0);

  g_az_val += static_cast<int>(control_variable_az);
  g_alt_val += static_cast<int>(control_variable_alt);

  servoWrite(constrain(g_az_val,0,180), 'z');
  servoWrite(constrain(g_alt_val,0,180), 'l');

  timer.tick();
}