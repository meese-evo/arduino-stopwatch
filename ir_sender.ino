#include <IRremote.h>

#define pinIR 3
#define statusPin 13
#define buttonPin 2

IRsend irsend;
int value = LOW;                    // previous value of the LED
int buttonState;                    // variable to store button state
int stateReset;
int lastButtonState;                // variable to store last button state
int blinking;                       // condition for blinking - timer is timing
long interval = 100;
long previousMillis = 0;            // variable to store last time LED was updated

void setup()
{
  pinMode(statusPin, OUTPUT);
  pinMode(buttonPin, INPUT);       // not really necessary, pins default to INPUT anyway
  digitalWrite(buttonPin, HIGH);
}

void loop() {
  buttonState = digitalRead(buttonPin); 
  
  if (buttonState == LOW && lastButtonState == HIGH && blinking == false){
    irsend.enableIROut(38);
    irsend.mark(0);
    digitalWrite(statusPin, HIGH);
    blinking = true;
    lastButtonState = buttonState;
  }
  
  else if (buttonState == LOW && lastButtonState == HIGH && blinking == true){
    digitalWrite(statusPin, LOW);
    digitalWrite(pinIR, LOW);
    blinking = false;
    lastButtonState = buttonState;
  }
  
  else{
    lastButtonState = buttonState;                         // store buttonState in lastButtonState, to compare next time
  }
  
  if ( (millis() - previousMillis > interval) ) {
  
    if (blinking == true){
      previousMillis = millis();                         // remember the last time we blinked the LED
  
      // if the LED is off turn it on and vice-versa.
      if (value == LOW)
        value = HIGH;
      else
        value = LOW;
      digitalWrite(statusPin, value);
      }
      else{
         digitalWrite(statusPin, LOW);                         // turn off LED when not blinking
      }
   }
  
  
}
