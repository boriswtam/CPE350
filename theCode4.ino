#include <QTRSensors.h>
#include <Servo.h> 

Servo myservo;  // create servo object to control a servo 

#define NUM_SENSORS  6      // number of sensors used
#define DEBUG 1
#define KP 0.2
#define KD 5

#define STRAIGHT_LINE_SPEED 100

#define DEFAULT_STRAIGHT_SPEED 120
#define MAX_STRAIGHT_SPEED 160

#define DEFAULT_TURN_SPEED 40
#define MAX_TURN_SPEED 80

#define DEFAULT_REVERSE_SPEED 40
#define MAX_REVERSE_SPEED 80

#define STRAIGHT 98
#define TURN 99

#define C_LINE_TO_LOAD 20  //35 cm from center line to loading zone
#define LOAD_TO_C_LINE 7
#define LOAD_TO_LOAD 59
#define LOAD_TO_WALL_NORTH_SIDE 20 //25 cm from horizontal loading zone line to wall, should be 25.4cm
#define LOAD_TO_WALL_SOUTH_SIDE 14
#define LOAD_TO_WALL_OPP 24
#define INTERSECT_TO_FIRST_NET 15 //approx 5 cm from intersection to center of first net

QTRSensorsRC qtrrcFront((unsigned char[]) {40,41,42,43,44,45}, NUM_SENSORS);
QTRSensorsRC qtrrcBack((unsigned char[]) {28,29,30,31,32,33}, NUM_SENSORS);

unsigned int sensorValues[NUM_SENSORS];

int minValFront[] = {640, 496, 424, 456, 424, 424}; 
int maxValFront[] = {2388, 1652, 1416, 1452, 1420, 1432};

int minValBack[] = {440, 420, 448, 448, 544, 744}; 
int maxValBack[] = {1308, 1204, 1264, 1264, 1568, 2144};

int lastError = 0;

// pins for the encoder inputs
#define LH_ENCODER_A 19
#define LH_ENCODER_B 22
#define RH_ENCODER_A 18
#define RH_ENCODER_B 23

#define FRONT_PROX 1
#define BACK_PROX 2

#define FORWARD  0
#define REVERSE 1
#define MOTOR_A 0 //right motor looking from the front
#define MOTOR_B 1

#define PWMA 10
#define DIRA1 8 
#define DIRA2 9

#define DIRB1 7
#define DIRB2 6
#define PWMB 5

#define RIGHT 0
#define LEFT 1

#define TURN_DEGREE 15.55 //28.27 cm

#define ONE_COUNT_DIST 0.01767 //28.27 cm / 1600

volatile long leftCount = 0;
volatile long rightCount = 0;

#define trigPinBack 36
#define echoPinBack 37
#define trigPinFront 50
#define echoPinFront 51
#define shootMotorPin 53
#define servoPin 13

void setup()
{
  Serial.begin(9600);
  /*pinMode(LH_ENCODER_A, INPUT);
  pinMode(LH_ENCODER_B, INPUT);
  pinMode(RH_ENCODER_A, INPUT);
  pinMode(RH_ENCODER_B, INPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(echoPinBack, INPUT);*/
  
  pinMode(trigPinFront, OUTPUT);
  pinMode(trigPinBack, OUTPUT);
  pinMode(shootMotorPin, OUTPUT);
  digitalWrite(shootMotorPin, HIGH);
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
 

  setupArdumoto(); // Set all pins as outputs
  attachInterrupt(digitalPinToInterrupt(LH_ENCODER_A), leftEncoderEvent, CHANGE); //pin 18 equates to hw interrupt 5
  attachInterrupt(digitalPinToInterrupt(RH_ENCODER_A), rightEncoderEvent, CHANGE); //pin 19 equates to hw interrupt 4
  qtrrcFront.calibrate();
  qtrrcBack.calibrate();
  for(int i=0; i<NUM_SENSORS; i++) {
    qtrrcFront.calibratedMinimumOn[i] = minValFront[i];
    qtrrcFront.calibratedMaximumOn[i] = maxValFront[i];
    qtrrcBack.calibratedMinimumOn[i] = minValBack[i];
    qtrrcBack.calibratedMaximumOn[i] = maxValBack[i];
  }
  delay(2000);
}

void loop()
{
  Serial.println(getProximity(trigPinBack, echoPinBack));
  /*shootFive();
  turnRight(90);
  loadBalls();
  turnRight(145);
  delay(2000);
  rotate(11.5, RIGHT, FRONT_PROX);
  shootFive();
  rotate(5, LEFT, FRONT_PROX);
  shootFive();
  rotate(11, LEFT, BACK_PROX);
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
  delay(10000);
*/
  //MAY BE ABLE TO VERIFY AT CORRECT SPOT USING CHECKINSTERSECT
  //so maybe make a function called drive DistCheckIntersect
  /*driveDist(FIRST_NET_TO_SEC_NET, FORWARD); 
  shootFive();
  driveDistUsingProxSens(SEC_NET_TO_INTERSECT, INTERSECT_TO_WALL, FORWARD);
  turnLeft90();
  loadBalls();
  turnRight90();
  //shoot 10 balls into four point net
  shootFive();
  shootFive();*/
}

void loadBalls() {
  driveUsingProxSens(C_LINE_TO_LOAD, LOAD_TO_WALL_NORTH_SIDE, FORWARD, STRAIGHT);
  driveUsingProxSens(LOAD_TO_LOAD, LOAD_TO_WALL_SOUTH_SIDE, REVERSE, REVERSE);
  driveUsingOppProxSens(LOAD_TO_C_LINE, LOAD_TO_WALL_OPP, FORWARD, STRAIGHT);
}

void shootFive() {
  digitalWrite(shootMotorPin, LOW);  // Added this line
  delay(250);
  for(int i=0; i < 5; i++) {
    myservo.write(120);
    delay(600);
    myservo.write(0);
    delay(300);
  }
  digitalWrite(shootMotorPin, HIGH);  // Added this line
}

void driveUsingOppProxSens(double initDist, double verifyDist, int dir, int movement) {
  driveDist(initDist, dir, movement); //using encoders
  unsigned int position;
  double distToWall;
  while(1) {
    if(dir == FORWARD)
      distToWall = getProximity(trigPinBack, echoPinBack);
    else
      distToWall = getProximity(trigPinFront, echoPinFront);
    Serial.println(distToWall);
    if(distToWall >= verifyDist)
      break;
    //otherwise, not quite there yet, keep going 
    if(dir == FORWARD)
      position = qtrrcFront.readLine(sensorValues);
    else
      position = qtrrcBack.readLine(sensorValues);
    setMotorSpeedLineDetect(sensorValues, position, dir, movement);
    //should put something to ensure that we wont run into wall if prox sensor is broken and reading high distances
  }
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
  leftCount = 0;
  rightCount = 0;
  delay(500);
}

void driveUsingProxSens(double initDist, double verifyDist, int dir, int movement) {
  driveDist(initDist, dir, movement); //using encoders
  unsigned int position;
  double distToWall;
  while(1) {
    if(dir == FORWARD)
      distToWall = getProximity(trigPinFront, echoPinFront);
    else
      distToWall = getProximity(trigPinBack, echoPinBack);
     Serial.println(distToWall);
    if(distToWall <= verifyDist)
      break;
    //otherwise, not quite there yet, keep going 
    if(dir == FORWARD)
      position = qtrrcFront.readLine(sensorValues);
    else
      position = qtrrcBack.readLine(sensorValues);
    setMotorSpeedLineDetect(sensorValues, position, dir, movement);
    //should put something to ensure that we wont run into wall if prox sensor is broken and reading high distances
  }
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
  leftCount = 0;
  rightCount = 0;
}

double getProximity(int trigPin, int echoPin) {
  long duration;
  double distance[10];
  double medianDist;
  for(int i=0; i < 10; i++) {
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance[i] = (duration/2) / 29.1;
  }
  medianDist = median(10, distance);
  return medianDist;
}

double median(int n, double x[]) {
    double temp;
    int i, j;
    // the following two loops sort the array x in ascending order
    for(i=0; i<n-1; i++) {
        for(j=i+1; j<n; j++) {
            if(x[j] < x[i]) {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }
    return((x[n/2] + x[n/2 - 1]) / 2.0);
}

void driveDist(int dist, int dir, int movement) {
  int distCount = dist/ONE_COUNT_DIST;
  unsigned int position;
  while (abs(rightCount) < distCount && abs(leftCount) < distCount) {
    if(dir == FORWARD)
      position = qtrrcFront.readLine(sensorValues);
    else
      position = qtrrcBack.readLine(sensorValues);
    setMotorSpeedLineDetect(sensorValues, position, dir, movement);
  }
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
  leftCount = 0;
  rightCount = 0;
}

void setMotorSpeedLineDetect(unsigned int sensorValues, unsigned int position, int dir, int movement) {
  
  int leftMotorSpeed, rightMotorSpeed, defaultSpeed, maxSpeed;
  int error = position - 2500;

  switch(movement) {
    case STRAIGHT:
      defaultSpeed = DEFAULT_STRAIGHT_SPEED;
      maxSpeed = MAX_STRAIGHT_SPEED;
    break;
    case TURN:
      defaultSpeed = DEFAULT_TURN_SPEED;
      maxSpeed = MAX_TURN_SPEED;
    break;
    case REVERSE:
      defaultSpeed = DEFAULT_REVERSE_SPEED;
      maxSpeed = MAX_REVERSE_SPEED;
    break;
  }
 
  /*if(checkIntersect(sensorValues)) {
      leftMotorSpeed = STRAIGHT_LINE_SPEED;
      rightMotorSpeed = STRAIGHT_LINE_SPEED;
  }*/
  //else {
    int motorSpeed = KP * error + KD * (error - lastError);
    
    lastError = error; //last error set as global
    
    leftMotorSpeed = defaultSpeed + motorSpeed;
    rightMotorSpeed = defaultSpeed - motorSpeed;    
    
    if(leftMotorSpeed < 0)
      leftMotorSpeed = 0;
    else if(leftMotorSpeed > maxSpeed)
      leftMotorSpeed = maxSpeed;
   
    if(rightMotorSpeed < 0)
      rightMotorSpeed = 0;
    else if(rightMotorSpeed > maxSpeed)
      rightMotorSpeed = maxSpeed;
  //}
  if(dir == FORWARD) {
    driveArdumoto(MOTOR_B, dir, leftMotorSpeed); 
    driveArdumoto(MOTOR_A, dir, rightMotorSpeed);
  }
  else {
    driveArdumoto(MOTOR_B, dir, leftMotorSpeed); 
    driveArdumoto(MOTOR_A, dir, rightMotorSpeed);
  }
}

bool checkIntersect(unsigned int sensorValues[]) {
  int count=0;
  for(int i=0; i< NUM_SENSORS; i++) {
    if(sensorValues[i] > 700)
      count++;
  }
  if(count > 3)
    return 1;
  return 0;
}

/*void tightRight() {  
  driveArdumoto(MOTOR_A, FORWARD, 127);
  while (abs(rightCount) < TURN_90*0.5);
  stopArdumoto(MOTOR_A);
  leftCount = 0;
  rightCount = 0;

  driveArdumoto(MOTOR_B, FORWARD, 127);
  while (abs(leftCount) < TURN_90*0.5);
  stopArdumoto(MOTOR_B);
  leftCount = 0;
  rightCount = 0;

  driveArdumoto(MOTOR_A, REVERSE, 50);
  while (abs(rightCount) < TURN_90*0.75);
  stopArdumoto(MOTOR_A);
  leftCount = 0;
  rightCount = 0;
}*/

void turnRight(int degree) {
  driveArdumoto(MOTOR_A, FORWARD, 50);  // Motor A at max speed
  while (rightCount < TURN_DEGREE*degree);
  leftCount=0;
  rightCount=0;
  stopArdumoto(MOTOR_A);  // STOP motor A 
}

void rotate(int deg, int dir, int prox_sensor) {
  if (prox_sensor == FRONT_PROX) {
    if(dir == RIGHT) {
      driveArdumoto(MOTOR_A, FORWARD, 50);
      driveArdumoto(MOTOR_B, REVERSE, 50);
      while (getProximity(trigPinFront, echoPinFront) > deg);
    }
    else {
      driveArdumoto(MOTOR_A, REVERSE, 50);
      driveArdumoto(MOTOR_B, FORWARD, 50);  
      while (getProximity(trigPinFront, echoPinFront) < deg);
    }
    //while (rightCount < TURN_DEGREE*deg && leftCount < TURN_DEGREE*deg);
  }
  else {
    if(dir == RIGHT) {
      driveArdumoto(MOTOR_A, FORWARD, 50);
      driveArdumoto(MOTOR_B, REVERSE, 50);
      while (getProximity(trigPinBack, echoPinBack) < deg);
    }
    else {
      driveArdumoto(MOTOR_A, REVERSE, 50);
      driveArdumoto(MOTOR_B, FORWARD, 50);  
      while (getProximity(trigPinBack, echoPinBack) > deg);
    }
  }
  stopArdumoto(MOTOR_A);  
  stopArdumoto(MOTOR_B);
  rightCount=0;
  leftCount=0;
}

/*void turnLeft(int degree) {
  driveArdumoto(MOTOR_B, dir, 60);  // Motor A at max speed
  while leftCount) < TURN_90);
  leftCount=0;
  stopArdumoto(MOTOR_B);  // STOP motor B
}*/

void leftEncoderEvent() {
  if (digitalRead(LH_ENCODER_A) == HIGH) {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount--;
    } else {
      leftCount++;
    }
  } else {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount++;
    } else {
      leftCount--;
    }
  }
}

// encoder event for the interrupt call
void rightEncoderEvent() {
  if (digitalRead(RH_ENCODER_A) == HIGH) {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount++;
    } else {
      rightCount--;
    }
  } else {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount--;
    } else {
      rightCount++;
    }
  }
}

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_A) {
    if (dir == REVERSE) {
      digitalWrite(DIRA1, HIGH);
      digitalWrite(DIRA2, LOW);
      analogWrite(PWMA, spd);
    }
    else {
      digitalWrite(DIRA1, LOW);
      digitalWrite(DIRA2, HIGH);
      analogWrite(PWMA, spd);
    }
  }
  else {
    if (dir == REVERSE) {
      digitalWrite(DIRB1, HIGH);
      digitalWrite(DIRB2, LOW);
      analogWrite(PWMB, spd);
    }
    else {
      digitalWrite(DIRB1, LOW);
      digitalWrite(DIRB2, HIGH);
      analogWrite(PWMB, spd);
    }
  }  
}

// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

// setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA1, OUTPUT);
  pinMode(DIRB1, OUTPUT);
  pinMode(DIRA2, OUTPUT);
  pinMode(DIRB2, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA1, LOW);
  digitalWrite(DIRB1, LOW);
  digitalWrite(DIRA2, LOW);
  digitalWrite(DIRB2, LOW);
}
