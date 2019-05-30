#include <Arduino.h>

const int motorPin3  = 48;
const int motorPin4  = 49;
const int enablepin34 = 4;
const int switchpin1 = 10;
const int switchpin2 = 11;
const int encoder0PinA  = 2;
const int encoder0PinB  = 3;
const int encoder0PinC  = 18;
const int encoder0PinD  = 19;
const int motorPin1  = 40;  // Pin 14 of L293
const int motorPin2  = 41;  // Pin 10 of L293
const int enablepin12 = 6;
const int switchpin3 = 8;
const int switchpin4 = 9;


int currentButton1 = 0;  //part of debounce function
int currentButton2 = 0;
boolean button1 = LOW;  //low puts driver into sleep mode, high turns it on
boolean button2 = LOW;
boolean button3 = LOW;
boolean button4 = LOW;
int print = 0;
int motor2move = 0;

long currentpos;
long steppos = 1000; //desired step length
long nsteppos = -1000;
long steps;

volatile long encoder0Pos1; 
volatile long encoder0Pos2; //gives position of encoder

// for timer
unsigned long currentmillis3 = 0;
unsigned long currentmillis2 = 0;
unsigned long currentmillis = 0;
unsigned long startmillis;
unsigned long startmillis2;
unsigned long startmillis3;
unsigned long stopperiod = 4000;
unsigned long stopperiod2 = 500;
unsigned long period = 2000;   //desired timing

int datafromUser = 0;

// for position correction
long setpoint;
long nsetpoint;
long input1;
long input2;
long output;

// interupt functions
void doEncoderA();
void doEncoderB();
void doEncoderC();
void doEncoderD();


void setup() {
   Serial.begin(9600);
  // put your setup code here, to run once:
  //pinMode(LED_BUILTIN, OUTPUT);
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
  
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enablepin12, OUTPUT);
  analogWrite(enablepin12, 35);
  pinMode(enablepin34, OUTPUT);
  analogWrite(enablepin34, 35);
  pinMode(switchpin1, INPUT_PULLUP);
  pinMode(switchpin2, INPUT_PULLUP);
  pinMode(switchpin3, INPUT_PULLUP);
  pinMode(switchpin4, INPUT_PULLUP);

  //start timer
  startmillis=millis();
}


void interface(){
  if(Serial.available() > 0)
  {
    datafromUser=Serial.read();
  }

  if(datafromUser == '0')
  {
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
    if (button1 == HIGH && currentButton1 == 1)
    {
    
    currentmillis=millis();
      if (currentmillis<=startmillis+period)
      {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        //Serial.println("3"); 
      } else
      {
        startmillis = currentmillis;               //resets timer
        encoder0Pos1 = 0;                           //resets position
        currentButton1 = 3;
      }
    }  
    if (currentButton1 == 1 || currentButton1 == 3)
    {
      
      if (input1 > nsetpoint + 5)                  // drives backward if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        print = 1;
      } else if (input1 < nsetpoint - 5)            // drives forward if not yet at step lengt (with an offset at 5)
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
          if (print == 1)
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
      if (button2 == HIGH)
      {
      currentmillis=millis();
        if (currentmillis<=startmillis+stopperiod)
        {
          motor2move = 1;
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
        } else
        {
          startmillis = currentmillis;               //resets timer
          encoder0Pos1 = 0;
          encoder0Pos2 = 0;                           //resets position
          currentButton1 = 2;
        }
      } 
    }
    if (currentButton1 == 2)
    {
      
      if (input1 < setpoint - 5)                  // drives backward if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        print = 1;
      } else if (input1 > setpoint + 5)            // drives forward if not yet at step lengt (with an offset at 5)
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
            Serial.print("3");
            Serial.print("\r\n");
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
        motor2move = 1;
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
      if (input2 > nsetpoint + 5)                  // drives backward if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin3, HIGH);
        digitalWrite(motorPin4, LOW);
      } else if (input2 < nsetpoint - 5)            // drives forward if not yet at step lengt (with an offset at 5)
      {
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, HIGH);

      } else                                       // brakes for 10s
      {
        digitalWrite(motorPin3, LOW);
        digitalWrite(motorPin4, LOW); 
        motor2move = 0; 
        //encoder0Pos2 = 0;
      }
    } 
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
  input1 = encoder0Pos1;
  input2 = encoder0Pos2; 
  interface();
  //Serial.println(encoder0Pos1, DEC);
  
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

