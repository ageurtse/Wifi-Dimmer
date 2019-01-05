// Voltage controlled dimmer with ATtiny85
//
// This arduino sketch includes a zero
// crossing detect function and an opto-isolated triac.
//
// AC Phase control is accomplished using the internal
// hardware timer1 in the ATtiny85
//
// Timing Sequence
// * timer is set up but disabled
// * zero crossing detected
// * timer starts counting from zero
// * comparator set to "delay to on" value
// * counter reaches comparator value
// * comparator ISR turns on triac gate
// * counter set to overflow - pulse width
// * counter reaches overflow
// * overflow ISR turns off triac gate
// * triac stops conducting at next zero cross

// The hardware timer runs at 1MHz (3mA).
// A half period of a 50Hz AC signal takes 10 ms is 10000 counts.
// @ 1MHZ (3ma)prescaler (1000) 128 78 counts per half period

#include <avr/io.h>
#include <avr/interrupt.h>
#include <SoftwareSerial.h>


#define DETECT PB2      //zero cross detect, interrupt 0
#define GATE PB3        //triac gate is physical
#define PULSE 4       	//trigger pulse width 4

#define RX    PB1   		//Serial port RX
#define TX    PB0   		//Serial port TX
SoftwareSerial Serial(RX, TX);

int dimvalue;			//After startup, turn off light;

void setup(){

  delay (9000);								//Wait for booting the ESP8266

	// set up pins
  pinMode(DETECT, INPUT);      //zero cross detect
  digitalWrite(DETECT, HIGH);  //enable pull-up resistor
  pinMode(GATE, OUTPUT);       //triac gate control


  //If all goes well, the light will flash several time, to notify the user
  digitalWrite(GATE, HIGH);
  delay(500);
  digitalWrite(GATE, LOW);
  delay(500);
  digitalWrite(GATE, HIGH);
  delay(500);
  digitalWrite(GATE, LOW);
  delay(500);
  digitalWrite(GATE, HIGH);


  // set up Timer1
  TCCR1 = 0;     											// stop timer
  OCR1A = dimvalue;										// set initial dim value
  TIMSK = _BV(OCIE1A) | _BV(TOIE1);  	// interrupt on Compare Match A | enable timer overflow interrupt
  sei();  														// enable interrupts
  // set up zero crossing interrupt
  attachInterrupt(0,zeroCrossingInterrupt, FALLING);   //

  // ***
  // *** Initialize the Serial port
  // ***
  Serial.begin(9600);
  while (!Serial) {
  }
}

//Interrupt Service Routines
void zeroCrossingInterrupt(){
  TCNT1 = 0;   							//reset timer - count from zero
  TCCR1 = B00001000;        // 1000 ajg 0110 for 64 see table 12.5 of the tiny85 datasheet
}

ISR(TIMER1_COMPA_vect){    //comparator match
  if(OCR1A>95){            //send no pulse to triac for low input voltage for 0v output
    return;
  }

	digitalWrite(GATE,HIGH); //set triac gate to high
  TCNT1 = 255-PULSE;       //trigger pulse width, when TCNT1=255 timer1 overflows
}

ISR(TIMER1_OVF_vect){ 			//timer1 overflow
  digitalWrite(GATE,LOW);   //turn off triac gate
  TCCR1 = 0;                //disable timer stop unintended triggers
}

void loop(){
  if (Serial.available()) {
    // read the most recent byte (which will be from 0 to 100, so it's in %):
  	dimvalue = Serial.read();
  	OCR1A = dimvalue;
  }
}
