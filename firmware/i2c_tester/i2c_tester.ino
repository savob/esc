#include <Wire.h>

byte address = 10;

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);

  Serial.println("Ready to send/request data. Format messages as [type r/w][register],[data (if writing)]");
  Serial.print("Target address is: ");
  Serial.println(address);
}

byte request = 0;
bool messageType = false;

void loop() {
  if (Serial.available()) {
    char temp = Serial.read();
    request = Serial.parseInt();
    
    if (temp == 'r') {
      // Read a register
      Serial.print("Reading register: ");
      Serial.println(request);

      Wire.beginTransmission(address);
      Wire.write(request);
      Wire.endTransmission();



      switch (request) {
        case 1:
          Wire.requestFrom(address, 1);
          Serial.print("Reverse: ");
          Serial.println(Wire.read());
          break;
        case 2:
          Wire.requestFrom(address, 1);
          Serial.print("Duty: ");
          Serial.println(Wire.read());
          break;
        case 3:
          Serial.println(Wire.requestFrom(address, 2));
          Serial.print("Current RPM: ");
          Serial.println(readWordWire());
          break;
        case 4:
          Wire.requestFrom(address, 1);
          Serial.print("Control Scheme: ");
          Serial.println(Wire.read());
          break;
        case 5:
          Wire.requestFrom(address, 2);
          Serial.print("Target RPM: ");
          Serial.println(readWordWire());
          break;
        case 6:
          Wire.requestFrom(address, 1);
          Serial.print("Number of cycles in rotation: ");
          Serial.println(Wire.read());
          break;
        case 7:
          Wire.requestFrom(address, 1);
          Serial.print("Motor status: ");
          Serial.println(Wire.read());
          break;
      }

    }
    else {
      // Write to a register
      int dataValue = Serial.parseInt(); // Get data to write

      if (request < 8) {
        Serial.print("Writing ");
        Serial.print(dataValue);
        Serial.print(" to register ");
        Serial.println(request);
        
        Wire.beginTransmission(address);
        Wire.write(request);
  
        switch (request) {
          case 2:
            Wire.write(byte(dataValue));
            break;
          case 4:
            Wire.write(byte(dataValue));
            break;
          case 5:
            sendWordWire(dataValue);
            break;
          case 6:
            Wire.write(dataValue);
            break;
          case 7:
            Wire.write(byte(dataValue));
            break;
        }
        Wire.endTransmission();
      }
      else {
        // LED and buzz (need a second data value
        unsigned int dataValue2 = Serial.parseInt();

        Serial.print("Writing ");
        Serial.print(word(dataValue));
        Serial.print(" and ");
        Serial.print(word(dataValue2));
        Serial.print(" to register ");
        Serial.println(request);

        Wire.beginTransmission(address);
        Wire.write(request);
  
        sendWordWire(dataValue);
        sendWordWire(dataValue2);
        
        Wire.endTransmission();

        
      }
    }

    Serial.println("DONE");

    while(Serial.available()) {
      Serial.read(); // Flush any extra characters
    }
  }
  delay(500);
}

void sendWordWire(word dataValue) {
  // Writes a word to wire interface
  // High byte first
  byte part1 = dataValue % 256;
  byte part2 = dataValue / 256;
  Wire.write(part2);
  Wire.write(part1);
}
word readWordWire() {
  byte part1 = Wire.read();
  byte part2 = Wire.read();
  word dataRecieved;
  dataRecieved = part1 * 256;
  dataRecieved += part2;

  return (dataRecieved);
}
