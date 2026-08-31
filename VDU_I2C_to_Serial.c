// Include Arduino Wire Library for I2C
#include <Wire.h>

// Defince Slave I2C Address
#define SLAVE_ADDR 0x28

// Define Slave answer size
#define ANSWER_SIZE 2

// Define terminating charactor
#define TERMINATOR 13

// Define pressure size
#define PRESSURE_SIZE 4

// Setup byte variable in the correct size
byte response[ANSWER_SIZE];
byte tempValue[ANSWER_SIZE];

String inputString = "";  // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

void receivedEvent() {
  // Read while data received
  while (0 < Wire.available()) {
    byte x = Wire.read();
  }

  // Print to Serial monitor
  Serial.println("Receive event");
}

void requestEvent() {
  // Send response back to Master
  Wire.write(response, ANSWER_SIZE);

  float value = response[1];
  value += response[0] * 256;

  // Print to Serial Monitor
//  Serial.print("Sent: ");
//  Serial.println(value);
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin(SLAVE_ADDR);

  // Function to run when data requested from master
  Wire.onRequest(requestEvent);

  // Function to run when data received from master
  Wire.onReceive(receivedEvent);

  // Setup Serial Monitor
  Serial.begin(115200);
  Serial.setTimeout(100);
  Serial.println("NGO Pressure Simulator");

  // Format answer as array
  response[0] = 0x06;
  response[1] = 0x5E;

  // reserve 100 bytes for the inputString:
  inputString.reserve(100);
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
/*
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if ((inChar == '\r') || (inChar == '\n')) {
      stringComplete = true;
    }
  }
}
*/

void loop() {
  // Update pressure when data received
  /*
  if (Serial.available() == 4) {
    // Read the incoming byte
    int bytesReceived = Serial.readBytes(tempValue, PRESSURE_SIZE);

    if (bytesReceived == ANSWER_SIZE) {
      int newPressure = tempValue[0] * 100 + tempValue[1] * 10 + tempValue[2];
      int newValue = (newPressure * 16383) / 150;
      if (newValue > 16383) {
        newValue = 16383;
      }
      response[0] = newValue / 256;
      response[1] = newValue & 0xFF;
    }
  }
  */

  /*
  if (stringComplete) {
    Serial.print("New pressure: ")
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  */


  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inputString += (char)inChar;
    }
    // if you get a carriage return, print the string's value:
    if (inChar == '\r') {
      Serial.print("Value: ");
      unsigned int newPressure = inputString.toInt();
      if (newPressure > 0x3FFF) {
        newPressure = 0x3FFF;
      }
      Serial.print(newPressure);

      response[0] = newPressure / 256;
      response[1] = newPressure % 256;
      Serial.print(" -> ");
      Serial.print(response[0]);
      Serial.print(" ");
      Serial.println(response[1]);
      // clear the string for new input:
      inputString = "";
    }
  }  
}
