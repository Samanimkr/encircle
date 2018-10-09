/** 
 * 
 * Author: Samani MK - @thesmklab 
 * 
 */

#include <medianFilter.h>
#include <NewPing.h>


// *********************************** PINS  *********************************** 

// LEFT MOTOR CONTROL PINS 
// - note - PWM pins must be used for motor control
const int leftMotorForwardPin = 10;
const int leftMotorBackwardPin = 12;
// RIGHT MOTOR CONTROL PINS 
const int rightMotorForwardPin = 7;
const int rightMotorBackwardPin = 6;

// LEFT ULTRASONIC SENSOR PINS
// - note - with my code, the echo pins for both sensors must be connected to pins which interrupts can be attached to
const unsigned char leftEchoPin = 2;
const unsigned char leftTriggerPin = 3;
// RIGHT ULTRASONIC SENSOR PINS
const unsigned char rightEchoPin = 18; 
const unsigned char rightTriggerPin = 19;

// *********************************** GLOBAL VARIABLES  *********************************** 

// Variables for storing motor speeds
int leftSpeed = 0;
int rightSpeed = 0;

// filter objects for ultrasonic sensors
medianFilter leftFilter;
medianFilter rightFilter;

NewPing leftSonar(leftTriggerPin, leftEchoPin, 200);
NewPing rightSonar(rightTriggerPin, rightEchoPin, 200);


// *********************************** MAIN PROGRAM ***********************************

void setup() 
{
  // set pin modes for motors
  pinMode(leftMotorForwardPin, OUTPUT);
  pinMode(leftMotorBackwardPin, OUTPUT);

  // set pin modes for motors
  pinMode(rightMotorForwardPin, OUTPUT);
  pinMode(rightMotorBackwardPin, OUTPUT);  
  
  // open serial port for USB communications
  Serial.begin(115200);

  Serial.println("*******************");
  Serial.println("Starting program");
  Serial.println("*******************");
    
}

void loop()
{
  // 1. Read from ultrasonic sensors
  float leftCM = leftSonar.ping_cm();
  float rightCM = rightSonar.ping_cm();
      
      setLeftMotor(40);
      setRightMotor(40);
      delay(700);
      pauserobot();
      setLeftMotor(40);
      setRightMotor(-40);
      delay(630);
      pauserobot();
      rightCM = rightSonar.ping_cm();
      leftCM = leftSonar.ping_cm();
      
      // print measurements to serial monitor
      Serial.println(leftCM);
      Serial.println(rightCM);
      Serial.println(); 


      //NOTE: YOU MAY HAVE TO CHANGE THE ROTATION AMOUNT OF THE
      //      MOTORS DEPENDING ON THE SURFACE THE ROBOT IS ON
      
      //Rotate cw to check how far the object is
      setLeftMotor(-40);
      setRightMotor(40);
      delay(630);
      pauserobot();

      // if the left sensor picks up that the object is close, then rotate
      // anti clockwise until its parallel to the object.
      // Then move forward.
      if(leftCM<=11){
        setLeftMotor(-40);
        setRightMotor(-40);
        delay(250);
        
        setLeftMotor(-40);
        setRightMotor(40);
        delay(350);
        pauserobot();

        //if the right sensor picks up that the object is too close,
        //then it will re-adjust by turning anti clockwise a lot. 
      } else if(rightCM>12){
        setLeftMotor(40);
        setRightMotor(40);
        delay(300);
        
        pauserobot();
        setLeftMotor(40);
        setRightMotor(0);
        delay(1100);
      }


// *********************************** MOTOR CONTROL FUNCTIONS BEGIN ***********************************


void pauserobot(){
  setLeftMotor(0);
  setRightMotor(0);
  delay(300);
}
 
void testMotors()
{
  // 2 seconds forwards
  setLeftMotor(40);
  setRightMotor(40);
  delay(2000);

  leftMotorStop();
  rightMotorStop();
  delay(1000);
  
  // 2 seconds backwards
  setLeftMotor(-40);
  setRightMotor(-40);
  delay(2000);

  leftMotorStop();
  rightMotorStop();
  delay(1000);

  // turn CCW
  setLeftMotor(-40);
  setRightMotor(40);
  delay(1000);

  leftMotorStop();
  rightMotorStop();
  delay(1000);

  // turn CW
  setLeftMotor(40);
  setRightMotor(-40);
  delay(1000);  

  leftMotorStop();
  rightMotorStop();
  delay(1000);  
}


void testMotors2()
{
  // 2 seconds forwards
//  setLeftMotor(40);
//  setRightMotor(40);
  setMotors(40, 40);
  delay(2000);

//  leftMotorStop();
//  rightMotorStop();
  stopMotors();  
  delay(1000);
  
  // 2 seconds backwards
//  setLeftMotor(-40);
//  setRightMotor(-40);
  setMotors(-40, -40);
  delay(2000);

//  leftMotorStop();
//  rightMotorStop();
  stopMotors();
  delay(1000);

  // turn CW
//  setLeftMotor(-40);
//  setRightMotor(40);
  setMotors(-40, 40);
  delay(1000);

//  leftMotorStop();
//  rightMotorStop();
  stopMotors();
  delay(1000);

  // turn CCW
//  setLeftMotor(40);
//  setRightMotor(-40);
  setMotors(40, -40);
  delay(1000);  

//  leftMotorStop();
//  rightMotorStop();
  stopMotors();
  delay(1000);  
}

/**
 * 
 * NOTE: THERE IS A FAIR AMOUNT OF DUPLICATED CODE HERE, WHICH IS NOT 
 * GOOD PROGRAMMING STYLE. 
 * I DON'T REALLY NEED TO HAVE SEPARATE FUNCTIONS FOR CONTROLLING THE LEFT AND
 * RIGHT MOTORS, BUT I HAVE LEFT IT THIS WAY BECAUSE IT IS EASIER TO READ
 * AND UNDERSTAND HOW IT ALL WORKS.
 * 
 */

void stopMotors()
{
  setMotors(0, 0);
}

/**
 * A function to set the speeds of both motors. See the comments for
 * setLeftMotor and setRightMotor 
 */
void setMotors(float leftDuty, float rightDuty)
{
  setLeftMotor(leftDuty);
  setRightMotor(rightDuty); 
}

/**
 * 
 * A function to set the left motor speed, in either the forwards or
 * backwards direction, depending on the sign of the argument duty.
 * 
 * Duty should be a float in the interval [-100,100]
 * 
 * For example: 
 * If duty = -50, the motor is turned in reverse at 50% power.
 * If duty = 100, the motor is turned in the forwards direction at full power.
 * If duty = 0, the motor is stopped.
 * 
 */
void setLeftMotor(float duty)
{
  if(duty > 0) // forwards
  {
    leftMotorForward(duty);
  }
  else if(duty == 0) // stop
  {
    leftMotorStop();
  }
  else // reverse
  {
    leftMotorBackward(-duty);  
  }
}


/**
 * 
 * A function to set the left motor to turn forwards with the specified 
 * duty cycle.
 * 
 */
void leftMotorForward(float duty)
{
  duty = constrainDuty(duty);
  analogWrite(leftMotorBackwardPin, 0);
  delay(10);
  analogWrite(leftMotorForwardPin, dutyToInt(duty));  
}

/**
 * 
 * A function to set the left motor to turn forwards with the specified 
 * duty cycle.
 * 
 */
void leftMotorBackward(float duty)
{
  duty = constrainDuty(duty);
  analogWrite(leftMotorForwardPin, 0);
  delay(10);
  analogWrite(leftMotorBackwardPin, dutyToInt(duty));   
}

/**
 * 
 * A function to stop the left motor
 * 
 */
void leftMotorStop()
{
  analogWrite(leftMotorForwardPin, 0);
  analogWrite(leftMotorBackwardPin, 0);
}

/**
 * 
 * A function to set the right motor speed, in either the forwards or
 * backwards direction, depending on the sign of the argument duty.
 * 
 * Duty should be a float in the interval [-100,100]
 * 
 * For example: 
 * If duty = -50, the motor is turned in reverse at 50% power.
 * If duty = 100, the motor is turned in the forwards direction at full power.
 * If duty = 0, the motor is stopped.
 * 
 */
void setRightMotor(float duty)
{
  if(duty > 0) // forwards
  {
    rightMotorForward(duty);
  }
  else if(duty == 0) // stop
  {
    rightMotorStop();
  }
  else // reverse
  {
    rightMotorBackward(-duty);  
  }
}

/**
 * 
 * A function to set the right motor to turn forwards with the specified 
 * duty cycle.
 * 
 */
void rightMotorForward(float duty)
{
  duty = constrainDuty(duty);
  analogWrite(rightMotorBackwardPin, 0);
  delay(10);
  analogWrite(rightMotorForwardPin, dutyToInt(duty));  
}

/**
 * 
 * A function to set the right motor to turn backwards with the specified 
 * duty cycle.
 * 
 */
void rightMotorBackward(float duty)
{
  duty = constrainDuty(duty);
  analogWrite(rightMotorForwardPin, 0);
  delay(10);
  analogWrite(rightMotorBackwardPin, dutyToInt(duty));   
}

/**
 * 
 * A function to stop the right motor
 * 
 */
void rightMotorStop()
{
  analogWrite(rightMotorForwardPin, 0);
  analogWrite(rightMotorBackwardPin, 0);
}

/**
 * 
 * A function to map the duty cycle values from the float interval [0,100]
 * to the integer interval required for analogWrite, which is [0,255]
 * 
 */
unsigned char dutyToInt(float duty)
{
  return 255 * duty / 100;
}

/**
 * 
 * A function to make sure duty cycle values are kept in the interval [0,100]
 * 
 */
float constrainDuty(float duty)
{
  if(duty > 100) duty = 100;
  else if(duty < 0) duty = 0;

  return duty;  
}

