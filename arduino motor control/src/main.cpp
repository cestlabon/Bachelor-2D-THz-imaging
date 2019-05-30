#include <Arduino.h>

//Defines all inputs
const int motorPin1  = 40;  
const int motorPin2  = 41;  
const int enablepin12 = 6;
const int switchpin1 = 10;
const int switchpin2 = 11;
const int encoder0PinA  = 2;
const int encoder0PinB  = 3;
const int motorPin3  = 48;
const int motorPin4  = 49;
const int enablepin34 = 4;
const int encoder0PinC  = 18;
const int encoder0PinD  = 19;
const int switchpin3 = 8;
const int switchpin4 = 9;

//Integers denoting which button has been pushed
int currentButton1 = 0;  
int currentButton2 = 0;

//Booleans defining the 4 switches
boolean button1 = LOW;  
boolean button2 = LOW;
boolean button3 = LOW;
boolean button4 = LOW;

//Integer defining whether er not somethin has been printed (for use in step part of the function)
int print = 0;

//integer defining whether or not motor 2 should take a step.
int motor2move = 0;


long steppos = 1000; //desired step length CW
long nsteppos = -1000; //desired step length CCW

//gives position of both encoders
volatile long encoder0Pos1; 
volatile long encoder0Pos2; 

// for timer
unsigned long currentmillis = 0;
unsigned long startmillis;
unsigned long stopperiod = 4000;
unsigned long stopperiod2 = 500;
unsigned long period = 2000;   

//to read serial port
int datafromUser = 0;

// for position correction
long setpoint;      //desired step length
long nsetpoint;     //desired negative steplenght
long input1;      
long input2;


// interupt functions
void doEncoderA();
void doEncoderB();
void doEncoderC();
void doEncoderD();


void setup() {
   Serial.begin(9600);
  // put your setup code here, to run once:
 
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(5, doEncoderC, CHANGE);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(4, doEncoderD, CHANGE);
  
  // defining motorpins and enablepins as OUTPUT
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enablepin12, OUTPUT);
  analogWrite(enablepin12, 35);
  pinMode(enablepin34, OUTPUT);
  analogWrite(enablepin34, 35);

  //defining switchpins as PULLUP
  pinMode(switchpin1, INPUT_PULLUP);
  pinMode(switchpin2, INPUT_PULLUP);
  pinMode(switchpin3, INPUT_PULLUP);
  pinMode(switchpin4, INPUT_PULLUP);

  //start timer
  startmillis=millis();
}


void measurement(){
  if(Serial.available() > 0)      //tjecks the serial port
  {
    datafromUser=Serial.read();   //reads the serial port
  }

  if(datafromUser == '0')
  {
    //home function for motor 1
    if (currentButton1 == 0)
    {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, HIGH);
      if (button1 == HIGH)
      {
        
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        currentButton1 = 1;
        encoder0Pos1 = 0;
        
      }
      
    }
    //home function for motor 2
    if (currentButton2 == 0)
    {
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, HIGH);

      if (button3 == HIGH)
      {
        
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, LOW);
        currentButton2 = 1;
        encoder0Pos2 = 0;
        
      }
    } 
  }
  else if (datafromUser == '1')
  {
    //function to escape the first button after homing (here motor will not be moving)
    if (button1 == HIGH && currentButton1 == 1)
    {    
    currentmillis=millis();                         //start current timer
      if (currentmillis<=startmillis+period)
      {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW); 
      } else
      {
        startmillis = currentmillis;               //resets timer
        encoder0Pos1 = 0;                           //resets position
        currentButton1 = 3;                        //sets direction and escapes the if statement
      }
    }  
    if (currentButton1 == 1 || currentButton1 == 3)   //CCW steps function for motor 1 
    {
      
      if (input1 > nsetpoint + 5)                  // drives CCW if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        print = 1;
      } else if (input1 < nsetpoint - 5)            // drives CW if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);

      } else                                       // brakes for 10s
      {      
      currentmillis=millis();
        if (currentmillis<=startmillis+period)
        {
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
          if (print == 1)                           //makes sure it only serial prints once
          {
            Serial.println("3");
            print = 0;
          }          
        } else
        {
          startmillis = currentmillis;               //resets timer
          encoder0Pos1 = 0;                           //resets position
        }
      }
      // function for hitting button 2
      if (button2 == HIGH)
      {
      currentmillis=millis();
        if (currentmillis<=startmillis+stopperiod)
        {
          motor2move = 1;                            //integer defining that motor 2 takes a step
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
        } else
        {
          startmillis = currentmillis;               //resets timer
          encoder0Pos1 = 0;                          //resets position
          encoder0Pos2 = 0;                           
          currentButton1 = 2;                        //sets direction and escapes the if statement 
        }
      } 
    }
    if (currentButton1 == 2)
    {
      
      if (input1 < setpoint - 5)                  // drives CW if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        print = 1;
      } else if (input1 > setpoint + 5)            // drives CCW if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);

      } else                                       // brakes for 10s
      {
      currentmillis=millis();
        if (currentmillis<=startmillis+period)
        {
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW); 
          if (print == 1)
          {
            Serial.println("3");                     //makes sure it only serial prints once
            print = 0;
          } 
        } else
        {
          startmillis = currentmillis;               //resets timer
          encoder0Pos1 = 0;                           //resets position
        }
      }
    }
    if (button1 == HIGH && currentButton1 == 2)
    {
    currentmillis=millis();
      if (currentmillis<=startmillis+period)
      {
        motor2move = 1;                              //integer defining that motor 2 takes a step
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW); 
      } else
      {
        startmillis = currentmillis;               //resets timer
        encoder0Pos1 = 0;
        encoder0Pos2 = 0;                           //resets position
        currentButton1 = 3;
      }
    }
    
    if (motor2move == 1)
    {
      if (input2 > nsetpoint + 5)                  // drives CCW if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin3, HIGH);
        digitalWrite(motorPin4, LOW);
      } else if (input2 < nsetpoint - 5)            // drives CW if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, HIGH);

      } else                                       // brakes for 10s
      {
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, LOW); 
        motor2move = 0; 
      }
    }
    //function for button 4 (brakes both motors and resets integers) 
    if (button4 == HIGH)
    {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
      currentButton1 = 0;
      currentButton2 = 0;
    }
    
     
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  button1 = digitalRead(switchpin1);
  button2 = digitalRead(switchpin2);
  button3 = digitalRead(switchpin3);
  button4 = digitalRead(switchpin4);
  setpoint = steppos;                       // desired step length
  nsetpoint = nsteppos;
  input1 = encoder0Pos1;                    // Position of encoder
  input2 = encoder0Pos2; 
  measurement();
  //Serial.println(encoder0Pos1, DEC);      // for decoding
  
}

//interrupt funtions
void doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos1 = encoder0Pos1 + 1;         // CW
    }
    else {
      encoder0Pos1 = encoder0Pos1 - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == HIGH) {
      encoder0Pos1 = encoder0Pos1 + 1;          // CW
    }
    else {
      encoder0Pos1 = encoder0Pos1 - 1;          // CCW
    }
  }
  //Serial.println (encoder0Pos, DEC);
  // use for debugging - remember to comment out
}

void doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {
      encoder0Pos1 = encoder0Pos1 + 1;         // CW
    }
    else {
      encoder0Pos1 = encoder0Pos1 - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {
      encoder0Pos1 = encoder0Pos1 + 1;          // CW
    }
    else {
      encoder0Pos1 = encoder0Pos1 - 1;          // CCW
    }
  }
}

void doEncoderC() {
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinC) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinD) == LOW) {
      encoder0Pos2 = encoder0Pos2 + 1;         // CW
    }
    else {
      encoder0Pos2 = encoder0Pos2 - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinD) == HIGH) {
      encoder0Pos2 = encoder0Pos2 + 1;          // CW
    }
    else {
      encoder0Pos2 = encoder0Pos2 - 1;          // CCW
    }
  }
  //Serial.println (encoder0Pos, DEC);
  // use for debugging - remember to comment out
}

void doEncoderD() {
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinD) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinC) == HIGH) {
      encoder0Pos2 = encoder0Pos2 + 1;         // CW
    }
    else {
      encoder0Pos2 = encoder0Pos2 - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinC) == LOW) {
      encoder0Pos2 = encoder0Pos2 + 1;          // CW
    }
    else {
      encoder0Pos2 = encoder0Pos2 - 1;          // CCW
    }
  }
}

