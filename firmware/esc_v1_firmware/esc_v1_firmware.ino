#include <avr/interrupt.h>
#include "i2c.h"
#include "motor.h"


void setup() {
  Serial.begin(115200); // Debugging/testing

  // Input / config pins
  DDRD  &= 0x03;  // Configure all input pins as inputs
  PORTD &= 0x03;  // Clear all input pullups
  PORTD |= 0xB8;  // Pull ups for all input solder pads (soldered ones short to GND)

  delay(1); // Allow outputs to settle before reading

  // Sets reverse to true if reverse pin is shorted
  reverse = ((PIND & 0x80) == 0);

  // Read I2C address settings
  byte temp = PIND & 0x38;  // Extract the 3 bits for setting
  temp ^= 0x38;             // Flips the bits (since I am shorting the pads I want to set as "1"
  temp = temp >> 3;         // Shift so bits are in spots 0 to 2 inclusive (0 to 7 decimal value)
  i2cAddress += temp;       // Add offest to default value

  Wire.begin(i2cAddress);
  Wire.onRequest(i2cRequest);
  Wire.onReceive(i2cRecieve);
  //  Serial.print("I2C Address of ESC is ");
  //  Serial.println(i2cAddress);

  // Disable pull-up resistors
  MCUCR |= (1 << PUD);
  // We don't pull-ups at any other point, so it's best
  // disabled pins stay hi-Z regardless of PORTx values

  // Analog comparator setting
  ADCSRA = (0 << ADEN);     // Disable the ADC module
  ADCSRB = (1 << ACME);     // Enable MUX select for negative input of comparator
  ACSR   = (0 << ACBG);     // Select PD6 as positive side for comparison

  disableMotor(); // Ensure motor is disabled at start

  Serial.println(F("START"));
}



void loop() {

  if (test2) { // Debugging printer
    Serial.println(F("xxx"));
    test2 = false;
    //Serial.println(sequenceStep);

    delay(5);
  }
  
  if (Serial.available()) {
    Serial.print(F("Current motor status: "));
    Serial.println(motorStatus);
    Serial.print(F("Current duty value is "));
    Serial.println(duty);
    
    int temp = Serial.parseInt();
    temp = constrain(temp, 0 , period);
    Serial.print(F("Input PWM was read as "));
    Serial.println(temp);
    
    enableMotor(temp);

    Serial.print(F("Resulting motor status: "));
    Serial.println(motorStatus);
    Serial.print(F("Resulting duty value is "));
    Serial.println(duty);
    Serial.println();
    
    delay(10000); // Run for a short period for now
  }

  disableMotor();
}
