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
int getSensorReadings();
int getSystemError(int sensor_1, int sensor_2, int sensor_3, int sensor_4, int setpoint);

Servo AzimuthServo;
Servo AltitudeServo;

auto timer = timer_create_default();

const int chipSelect = 4;
int g_val;    // variable to read the value from the analog pin
unsigned long g_systick = 0;
const long beat = 1000;
int g_az_val = 90;
int g_alt_val =  90;
int g_setpoint_ref = 0;

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
  int photo_resistor[4];
  photo_resistor = getSensorReadings();

  int setpoint_reference = 0;
  int azimuth_error = getSystemError(photo_resistor[1],photo_resistor[2],photo_resistor[0],photo_resistor[3], g_setpoint_ref);
  //int altitude_feedback = getSystemError();
  int altitude_feedback = (average(photo_resistor[0],photo_resistor[1]) - average(photo_resistor[2],photo_resistor[3]));
  int altitude_error = g_setpoint_ref - altitude_feedback;
  
  // TO DO: Add integral and derivative
  int control_variable_az = K_azimuth.K_p*azimuth_error + K_azimuth.K_i*(0) + K_azimuth.K_d*(0);
  int control_variable_alt = K_altitude.K_p*altitude_error + K_altitude.K_i*(0) + K_altitude.K_d*(0);

  g_az_val += control_variable_az;
  g_alt_val += control_variable_alt;

  servoWrite(constrain(g_az_val,0,180), 'z');
  servoWrite(constrain(g_alt_val,0,180), 'l');

  timer.tick();
}
