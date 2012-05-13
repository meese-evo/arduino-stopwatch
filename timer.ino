#include <LiquidCrystal.h>


#define ledPin  10
#define startstopPin 6
#define resetPin 7

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int value = LOW;                    // previous value of the LED
int buttonState;                    // variable to store button state
int stateReset;
int lastButtonState;                // variable to store last button state
int blinking;                       // condition for blinking - timer is timing
long interval = 100;                // blink interval - change to suit
long previousMillis = 0;            // variable to store last time LED was updated
long startTime ;                    // start time for stop watch
long elapsedTime ;                  // elapsed time for stop watch
int fractional;                     // variable used to store fractional part of time



void setup()
{
   Serial.begin(9600);

   pinMode(ledPin, OUTPUT);         // sets the digital pin as output

   pinMode(startstopPin, INPUT);       // not really necessary, pins default to INPUT anyway
   pinMode(resetPin, INPUT);
   digitalWrite(startstopPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.
   digitalWrite(resetPin, HIGH);
   
   // set up the LCD's number of columns and rows: 
   lcd.begin(20, 4);
   // Print a message to the LCD.
   lcd.print("   --Stop Watch--");
   lcd.setCursor(0, 1);
   lcd.print("System bereit!");

}

void loop()
{
    // check for button press
   buttonState = digitalRead(startstopPin);                   // read the button state and store
   stateReset = digitalRead(resetPin);

   if (buttonState == LOW && lastButtonState == HIGH && blinking == false){     // check for a high to low transition
      // if true then found a new button press while clock is not running - start the clock

      startTime = millis();                                   // store the start time
      blinking = true;                                        // turn on blinking while timing
      delay(5);                                               // short delay to debounce switch
      lastButtonState = buttonState;      // store buttonState in lastButtonState, to compare next time
      
      lcd.clear();
      lcd.print("   --Stop Watch--");
      lcd.setCursor(0, 1);
      lcd.print("Zeitnahme gestartet!");
   }

   else if (buttonState == LOW && lastButtonState == HIGH && blinking == true){     // check for a high to low transition
      // if true then found a new button press while clock is running - stop the clock and report

        elapsedTime =   millis() - startTime;              // store elapsed time
        blinking = false;                                  // turn off blinking, all done timing
        lastButtonState = buttonState;                     // store buttonState in lastButtonState, to compare next time

       // routine to report elapsed time
        Serial.print( (int)(elapsedTime / 1000L));         // divide by 1000 to convert to seconds - then cast to an int to print

        Serial.print(".");                             // print decimal point

        // use modulo operator to get fractional part of time
       fractional = (int)(elapsedTime % 1000L);

       // pad in leading zeros - wouldn't it be nice if
       // Arduino language had a flag for this? :)
       if (fractional == 0)
          Serial.print("000");      // add three zero's
       else if (fractional < 10)    // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros
          Serial.print("00");       // add two zeros
       else if (fractional < 100)
          Serial.print("0");        // add one zero

       Serial.println(fractional);  // print fractional part of time
       
       // set the cursor to column 0, line 2
       // (note: line 1 is the second row, since counting begins with 0):
       lcd.clear();
       lcd.print("   --Stop Watch--");
       
       lcd.setCursor(0, 1);
       lcd.print("Lauf beendet! Zeit:");
       
       lcd.setCursor(0, 2);
       lcd.print((int)(elapsedTime / 1000));
       lcd.print(" sec");
       
       lcd.setCursor(0, 3);
       lcd.print(fractional);
       lcd.print(" ms");
   }

   else{
      lastButtonState = buttonState;                         // store buttonState in lastButtonState, to compare next time
   }
   
   if (stateReset == LOW) {
     blinking = false;
     lcd.clear();
     lcd.print("   --Stop Watch--");
     lcd.setCursor(0, 1);
     lcd.print("System bereit!");
  }

   // blink routine - blink the LED while timing
   // check to see if it's time to blink the LED; that is, the difference
   // between the current time and last time we blinked the LED is larger than
   // the interval at which we want to blink the LED.

   if ( (millis() - previousMillis > interval) ) {

      if (blinking == true){
         previousMillis = millis();                         // remember the last time we blinked the LED

         // if the LED is off turn it on and vice-versa.
         if (value == LOW)
            value = HIGH;
         else
            value = LOW;
         digitalWrite(ledPin, value);
      }
      else{
         digitalWrite(ledPin, LOW);                         // turn off LED when not blinking
      }
   }

}
