/*  Include Needed Library    */
#include <Servo.h>

/*  Useful Defines  */
#define DISPENSER_PIN       9
#define LED_INDECATION_OFF  8
#define LED_INDECATION_ON   7
#define ECHO                6
#define TRIGG               5
#define SOUD_SPEED_CM       0.034
#define DISP_ON             255
#define DISP_OFF            0
#define THRESHOLD_DISTANCE_CM 15  

/*    Static Global variable of distance in CM*/
static int DistanceInCM = 0;
static bool terminated = true;
static long dropCounter = 0;
/*  Created Object  */
Servo Disp;

/*  Prototypes    */
int getDistanceInCM (void);

/*  Setting Up  */
void setup() {
  /*  Serial for Debugging purposes   */
  Serial.begin(9600);
  /*  attach servo to pin number DISPENSER_PIN (9)*/
  Disp.attach(DISPENSER_PIN);
  /*  Setting up pins directions    */
  pinMode (LED_INDECATION_ON, OUTPUT);
  pinMode (LED_INDECATION_OFF, OUTPUT);
  pinMode (ECHO, INPUT);
  pinMode (TRIGG, OUTPUT);

  Serial.println("(Debug)Initialized Successfully...");
}

void loop() {
  /*    Get the distance in CM    */
  DistanceInCM = getDistanceInCM() ;
  /*    Check the Distance below or above 15 cm    */
  if ( DistanceInCM >= THRESHOLD_DISTANCE_CM ) {
    digitalWrite(LED_INDECATION_ON, LOW);
    digitalWrite(LED_INDECATION_OFF, HIGH);
    Disp.write(DISP_OFF);
    terminated = true;
  }
  else if ((DistanceInCM < THRESHOLD_DISTANCE_CM) &&( terminated == true )) {
    digitalWrite(LED_INDECATION_OFF, LOW);
    digitalWrite(LED_INDECATION_ON, HIGH);
    Disp.write(DISP_ON);
    delay(500);
    Disp.write(DISP_OFF);
    delay(350);
    Disp.write(DISP_ON);
    delay(500);
    Disp.write(DISP_OFF);
    terminated = false;
    dropCounter++;
  }
  delay(300);
}

int getDistanceInCM (void) {
  /*   decleration variables    */
  long duration; // variable for the duration of sound wave travel
  int distanceInCM; // variable for the distance measurement

  /*   trigg for 10 us    */
  digitalWrite(TRIGG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGG, LOW);
  /*   get Time in us    */
  duration = pulseIn(ECHO, HIGH);
  distanceInCM = duration * SOUD_SPEED_CM / 2;
  Serial.println("(Debug) distanceInCM : " + (String)distanceInCM);
  return distanceInCM;
}
