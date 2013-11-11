/* 
 Sumorobot program
 
 Written in less than 48 hours due to a hardware issue
 
 Teensy 2 and TB6612FNG motordriver IC, Ping sonar for opponent detection and two IR sensors for edge detection
 
 GUNNU GPL- etc
 */

#include <QTRSensors.h>
#include <NewPing.h>


//Setting up the motor connections

const int pwmA = 10;
const int pwmB = 9;
const int ainTwo = 11; 
const int ainOne = 12;
const int binOne = 13;
const int binTwo = 14;
const int stby = 8;

//Sensors
//IR reflective sensors
#define NUM_SENSORS   2     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define LIGHT_TRIGGER 80
//90 seems to be a good number to trigger. 
QTRSensorsRC qtrrc((unsigned char[]) {5,6}, NUM_SENSORS, TIMEOUT); 
unsigned int sensorValues[NUM_SENSORS];



//Ping))) sensor
#define TRIGGER_PIN  7
#define MAX_DISTANCE 55 //31cm about one foot

NewPing sonar(TRIGGER_PIN, TRIGGER_PIN, MAX_DISTANCE);   

//***************************Main program********************************
/*---------------------------init and startup--------------------------*/
  delay(5000);
  //Init all motor control pins
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(ainOne, OUTPUT);
  pinMode(ainTwo, OUTPUT);
  pinMode(binOne, OUTPUT);
  pinMode(binTwo, OUTPUT);
  pinMode(stby, OUTPUT);
  Serial.begin(115200); // Open serial monitor at 9600 baud to see data
}



void loop() {
  
  scan();
    
   
}


//***************************FUNCTIONS / SUB ROUTINES********************************
/*---------------------------DRIVE FUNCTIONS---------------------------------------*/

//Forward
void driveForward(){
  analogWrite(pwmA, 255);
  analogWrite(pwmB, 255);
  digitalWrite(ainOne, HIGH);
  digitalWrite(ainTwo, LOW);
  digitalWrite(binOne, HIGH);
  digitalWrite(binTwo, LOW);
  digitalWrite(stby, HIGH);
  return;
}

//Stop
void driveStop() {
  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);
  digitalWrite(ainOne, LOW);
  digitalWrite(ainTwo, LOW);
  digitalWrite(binOne, LOW);
  digitalWrite(binTwo, LOW);
  digitalWrite(stby, LOW);
  return;
}

//Reverse  

void driveReverse(){
  analogWrite(pwmA, 255);
  analogWrite(pwmB, 255);
  digitalWrite(ainOne, LOW);
  digitalWrite(ainTwo, HIGH);
  digitalWrite(binOne, LOW);
  digitalWrite(binTwo, HIGH);
  digitalWrite(stby, HIGH);
  return;
} 

//turn Left
void turnLeft(){
  analogWrite(pwmA, 127);
  analogWrite(pwmB, 127);
  digitalWrite(ainOne, LOW);
  digitalWrite(ainTwo, LOW);
  digitalWrite(binOne, HIGH);
  digitalWrite(binTwo, LOW);
  digitalWrite(stby, HIGH);
}

void turnRight(){
  analogWrite(pwmA, 127);
  analogWrite(pwmB, 127);
  digitalWrite(ainOne, HIGH);
  digitalWrite(ainTwo, LOW);
  digitalWrite(binOne, LOW);
  digitalWrite(binTwo, LOW);
  digitalWrite(stby, HIGH);
}


//reverse
void motorReverse(){
  analogWrite(pwmA, 127);
  analogWrite(pwmB, 127);
  digitalWrite(ainOne, LOW);
  digitalWrite(ainTwo, HIGH);
  digitalWrite(binOne, LOW);
  digitalWrite(binTwo, HIGH);
  digitalWrite(stby, HIGH);
  
}
  
/*---------------------------opponent/edge detection---------------------------------------*/
void scan(){  
  
  int distance = sonar.ping_cm(); 
  if (distance <= MAX_DISTANCE && distance != 0){
    
    qtrrc.read(sensorValues);
    driveForward();
    delay(20);
    lightSense();
  }else{
    
    qtrrc.read(sensorValues);
    turnLeft();
    delay(20);
    lightSense(); 
  }
} 


void lightSense(){
if (sensorValues[0] < LIGHT_TRIGGER)
{
    motorReverse();
    delay(1000);
    turnLeft();
    delay(750);
  }
  else if (sensorValues[1] < LIGHT_TRIGGER)
  {
 
    motorReverse();
    delay(1000);
    turnRight();
    delay(750);
  } else {
    scan();
  }
}  
