#include <QTRSensors.h>
#include <Servo.h> 

Servo myservo;  // create servo object to control a servo 

#define NUM_SENSORS  6      // number of sensors used
#define DEBUG 1
#define KP 0.2
#define KD 5

#define DEFAULT_STRAIGHT_SPEED 80
#define MAX_STRAIGHT_SPEED 120

#define STRAIGHT_LINE_SPEED 80

#define DEFAULT_TURN_SPEED 40
#define MAX_TURN_SPEED 60

#define STRAIGHT 98
#define TURN 99

#define C_LINE_TO_LOAD 20 //35 cm from center line to loading zone
#define LOAD_TO_C_LINE 8.5
#define LOAD_TO_LOAD 60
#define LOAD_TO_WALL_FUN_SIDE 24.5 //25 cm from horizontal loading zone line to wall, should be 25.4cm
#define LOAD_TO_WALL_NONFUN_SIDE 23.5
#define WALL_TO_TURN_POINT 15
#define INTERSECT_TO_FIRST_NET 10 //approx 5 cm from intersection to center of first net
#define D_TO_C 30

QTRSensorsRC qtrrcFront((unsigned char[]) {40,41,42,43,44,45}, NUM_SENSORS);
QTRSensorsRC qtrrcBack((unsigned char[]) {33,32,31,30,29,28}, NUM_SENSORS);

unsigned int sensorValues[NUM_SENSORS];

int minValFront[] = {640, 496, 424, 456, 424, 424}; 
int maxValFront[] = {2388, 2224, 1608, 1340, 1424, 1380, 1356};

int minValBack[] = {440, 420, 448, 448, 544, 744}; 
int maxValBack[] = {1308, 1204, 1264, 1264, 1568, 2144}; 

int lastError = 0;

// pins for the encoder inputs
#define LH_ENCODER_A 19
#define LH_ENCODER_B 22
#define RH_ENCODER_A 18
#define RH_ENCODER_B 23

#define FORWARD  0
#define REVERSE 1
#define MOTOR_A 0
#define MOTOR_B 1

#define PWMB 10
#define DIRB1 9 
#define DIRB2 8

#define DIRA1 6
#define DIRA2 7
#define PWMA 5

#define TURN_90 1400 //28.27 cm

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
  //first pass
  loadBalls();
  tightRight();
  driveDist(INTERSECT_TO_FIRST_NET, TURN, REVERSE);
  /*shootFive();
  driveDist(D_TO_C, STRAIGHT, REVERSE); 
  shootFive();*/
  
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
  delay(10000);

  //when testing driveDistToIntersect, take out the check intersect part to make sure FIRST_NET_TO_SEC_NET
  //is approximately the correct distance
  
  /*driveUsingProxSens(SEC_NET_TO_INTERSECT, INTERSECT_TO_WALL, FORWARD);
  turnLeft90();
  loadBalls();
  turnRight90();
  //shoot 10 balls into four point net
  shootFive();
  shootFive();*/
}

void loadBalls() {
  driveDist(C_LINE_TO_LOAD, STRAIGHT, FORWARD);
  driveDist(LOAD_TO_LOAD, STRAIGHT, REVERSE);
  driveDist(LOAD_TO_C_LINE, STRAIGHT, FORWARD);
  /*driveUsingProxSens(C_LINE_TO_LOAD, LOAD_TO_WALL_FUN_SIDE, FORWARD, STRAIGHT);
  driveUsingProxSens(LOAD_TO_LOAD, LOAD_TO_WALL_NONFUN_SIDE, REVERSE, STRAIGHT);
  driveUsingOppProxSens(LOAD_TO_C_LINE, WALL_TO_TURN_POINT, FORWARD, STRAIGHT);*/
}

void shootFive() {
  digitalWrite(shootMotorPin, LOW);  // Added this line
  delay(250);
  for(int i=0; i < 8; i++) {
    myservo.write(70);
    delay(250);
    myservo.write(10);
    if (i != 7)
      delay(1000);
  }
  digitalWrite(shootMotorPin, HIGH);  // Added this line
}

/***********************
drive a certain distance then if the opposite proximity sensor is too close,
i.e. we haven't driven far enough, continue driving till we are a certain
distance away from the wall
**********************/
void driveUsingOppProxSens(double initDist, double verifyDist, int dir, int movement) {
  unsigned int position;
  driveDist(initDist, dir, movement); //using encoders
  double distToWall;
  while(1) {
    if(dir == FORWARD)
      distToWall = getProximity(trigPinBack, echoPinBack);
    else
      distToWall = getProximity(trigPinFront, echoPinFront);
    if(distToWall >= verifyDist)
      break;
    //otherwise, not quite there yet, keep going 
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

void driveUsingProxSens(double initDist, double verifyDist, int dir, int movement) {
  unsigned int position;
  driveDist(initDist, dir, movement); //using encoders
  double distToWall;
  while(1) {
    if(dir == FORWARD)
      distToWall = getProximity(trigPinFront, echoPinFront);
    else
      distToWall = getProximity(trigPinBack, echoPinBack);
    if(distToWall <= verifyDist)
      break;
    //otherwise, not quite there yet, keep going 
    if(dir == FORWARD)
      position = qtrrcFront.readLine(sensorValues);
    else
      position = qtrrcBack.readLine(sensorValues);
    setMotorSpeedLineDetect(sensorValues, position, dir, movement);
    //should put something to ensure that we wont run into wall if prox sensor is broken and reading high distances
    //delay(50);
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
    //delay(5);
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

void driveDist(int dist, int movement, int dir) {
  int distCount = dist/ONE_COUNT_DIST;
  unsigned int position;
  while (abs(rightCount) < distCount && abs(leftCount) < distCount) {
    if(dir == FORWARD) {
      position = qtrrcFront.readLine(sensorValues);
    }
    else
      position = qtrrcBack.readLine(sensorValues);
    setMotorSpeedLineDetect(sensorValues, position, dir, movement);
  }
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
  leftCount = 0;
  rightCount = 0;
}

int setMotorSpeedLineDetect(unsigned int sensorValues, unsigned int position, int dir, int movement) {
  int error = position - 2500;
  int leftMotorSpeed, rightMotorSpeed;
  if(checkIntersect(sensorValues)) {
      leftMotorSpeed = STRAIGHT_LINE_SPEED;
      rightMotorSpeed = STRAIGHT_LINE_SPEED;
  }
  else {
    int motorSpeed = KP * error + KD * (error - lastError);
    
    lastError = error; //last error set as global
    if(movement == STRAIGHT) {
      leftMotorSpeed = DEFAULT_STRAIGHT_SPEED + motorSpeed;
      rightMotorSpeed = DEFAULT_STRAIGHT_SPEED - motorSpeed;
    }
    else {
      leftMotorSpeed = DEFAULT_TURN_SPEED + motorSpeed;
      rightMotorSpeed = DEFAULT_TURN_SPEED - motorSpeed;    
    }
    
    if(leftMotorSpeed < 0)
      leftMotorSpeed = 0;
    else if(leftMotorSpeed > MAX_STRAIGHT_SPEED && movement == STRAIGHT)
      leftMotorSpeed = MAX_STRAIGHT_SPEED;
    else if(leftMotorSpeed > MAX_TURN_SPEED && movement == TURN)
      leftMotorSpeed = MAX_TURN_SPEED;

        
    if(rightMotorSpeed < 0)
      rightMotorSpeed = 0;
    else if(rightMotorSpeed > MAX_STRAIGHT_SPEED && movement == STRAIGHT)
      rightMotorSpeed = MAX_STRAIGHT_SPEED;
    else if(rightMotorSpeed > MAX_TURN_SPEED && movement == TURN)
      rightMotorSpeed = MAX_TURN_SPEED;
  }
  driveArdumoto(MOTOR_B, dir, leftMotorSpeed); 
  driveArdumoto(MOTOR_A, dir, rightMotorSpeed);
  /*Serial.println(leftMotorSpeed); // comment this line out if you are using raw values
  Serial.println(rightMotorSpeed); // comment this line out if you are using raw values
  Serial.println();*/
  return 0;
}

bool checkIntersect(unsigned int sensorValues[]) {
  int count=0;
  for(int i=0; i< NUM_SENSORS; i++) {
    if(sensorValues[i] > 600)
      count++;
  }
  if(count > 2)
    return 1;
  return 0;
}

void tightRight() {  
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
}

void turnRight90(int dir) {
  delay(100);
  driveArdumoto(MOTOR_A, dir, 127);  // Motor A at max speed
  while (abs(rightCount) < TURN_90);
  rightCount=0;
  stopArdumoto(MOTOR_A);  // STOP motor A 
  delay(500);
}

void turnLeft90(int dir) {
  delay(100);
  driveArdumoto(MOTOR_B, dir, 127);  // Motor A at max speed
  while (abs(leftCount) < TURN_90);
  leftCount=0;
  stopArdumoto(MOTOR_B);  // STOP motor B
  delay(250);
}

void getCount() {
  Serial.print("Right Count: ");
  Serial.println(rightCount);
  Serial.print("Left Count: ");
  Serial.println(leftCount);
  Serial.println();
}

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
