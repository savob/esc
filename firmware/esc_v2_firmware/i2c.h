#include <Wire.h>

byte i2cAddress = 10;      // I2C address. Starts with this default, then adds offset according to soldering pads
byte currentInstruction = 0;


void i2cRecieve(int howMany);
void i2cRequest();
void sendWordWire(word dataValue);
word readWordWire();
