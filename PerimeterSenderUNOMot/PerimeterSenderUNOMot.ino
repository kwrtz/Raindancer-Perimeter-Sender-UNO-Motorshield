#include "TimerOne.h"

/*************************************************************
Motor Shield as Sender
Die Schleife wird an Motor B angeschlossen. Ggf. mit einem
Leistungsvorwiderstand von 47Ohm zum testen. Der Arduino muss mit
externer Spannung versorgt werden.
*************************************************************/
#define PAUSE 104


int adirection = 12;  //PB4  //IN1
int abrake = 9;  //PB1  
int apwm = 3;  //PD3         //ENA
int currentsensingA = A0;

int bdirection = 13; //PB5    //IN3
int bbrake = 8;  //PB0
int bpwm = 11;    //PB3          //ENB
int currentsensingB = A1;

volatile int step = 0;
volatile boolean enableSender = true;

#ifdef USE_DEVELOPER_TEST
  // a more motor driver friendly signal (sender)
  int8_t sigcode[] = {  1,0,0,0,0,
                        1,0,0,0,0,
                       -1,0,0,0,0,
                        1,0,0,0,0   };
#else
 // int8_t sigcode[] = { 1,1,-1,-1,1,-1,1,-1,-1,1,-1,1,1,-1,-1,1,-1,-1,1,-1,-1,1,1,-1 }; // 24 Zahlen
// int8_t sigcode[] = {-1,1,1,-1,1,-1,-1,1,1,-1,-1,1,1,-1,-1,1,1,-1,-1,1,-1,1,1,-1,-1,1,1,-1,-1,1,1,-1,1,-1,-1,1,-1,1,1,-1,-1,1,1,-1,1,-1,-1,1,1,-1,-1,1,-1,1,1,-1,1,-1,-1,1};  // 60 Zahlen
 int8_t sigcode[] = { 1, 1, -1, -1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, 1, -1, 1, -1, 1, -1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, 1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, 1, -1, -1, 1, -1,
  -1, 1, 1, -1, 1, 1, -1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, 1, -1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, -1, -1, 1, 1, -1, 1, 1, -1, -1, 1
}; //128 Zaahlen
#endif

void timerCallback(){       
  if (enableSender){
    if (sigcode[step] == 1) {      
      PORTB |= (1 << PB5);      //High
      PORTB |= (1 << PB3);      // Enable PWM
    } else if (sigcode[step] == -1) {              
       PORTB &= ~(1 << PB5);     //Low
       PORTB |= (1 << PB3); 
       PORTB |= (1 << PB3);      // Enable PWM
    } 
    step ++;    
    if (step == sizeof sigcode) {      
      step = 0;      
    }    
  } else {
    PORTB &= ~(1 << PB3); // Disable PWM
  }
}


void setup() {

  //Setup Channel B
  pinMode(bdirection, OUTPUT); //Initiates Motor Channel B pin
  pinMode(bbrake, OUTPUT);     //Initiates Brake Channel B pin
  pinMode(bpwm, OUTPUT);     //Initiates Speed Channel B pin (normaly is this the PWM Pin)
  digitalWrite(bbrake, LOW);   //Disengage the Brake for Channel B

  pinMode(adirection, OUTPUT); //Initiates Motor Channel A pin
  pinMode(abrake, OUTPUT);     //Initiates Brake Channel A pin
  pinMode(apwm, OUTPUT);     //Initiates Speed Channel A pin (normaly is this the PWM Pin)
  digitalWrite(adirection, LOW);   //Disengage the Brake for Channel A

 // sample rate 9615 Hz
  long  T = 104;
  Serial.begin(19200);
  
  Serial.println("START");
  Serial.print("Ardumower Sender ");
  
  
  Timer1.initialize(T);         // initialize timer1, and set period
  //Timer1.pwm(pinPWM, 256);  
  Timer1.attachInterrupt(timerCallback);  

}

void loop() {

 

}


