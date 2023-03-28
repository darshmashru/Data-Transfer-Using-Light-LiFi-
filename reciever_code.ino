#include <Arduino.h>

const int photoResistorPin = A1; // Use A1 pin for Arduino Uno
const int threshold = 256;
const int bitRate = 100;
const String startBits = "00000010";
const String endBits = "00000011";

void setup() {
  pinMode(photoResistorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Wait for start bits
  bool startFound = false;
  String receivedBits = "";
  while (!startFound) {
    int reading = analogRead(photoResistorPin);
    // Serial.println(analogRead(photoResistorPin));
    // Check for start bits
    if (reading > threshold) {
      receivedBits += "1";
    } else {
      receivedBits += "0";
    }
    delay(bitRate);
    
    if (receivedBits.length() > 8) {
      receivedBits = receivedBits.substring(receivedBits.length() - startBits.length(), receivedBits.length());
    }
    if (receivedBits == startBits)
    {
      startFound = true;
    }
  }
  // Read message bits
  bool endFound = false;
  while (!endFound) {
    int reading = analogRead(photoResistorPin);
    if (reading > threshold) {
      receivedBits += "1";
    } else {
      receivedBits += "0";
    }
    delay(bitRate);
    // Check for end bits
    if (receivedBits.substring(8).endsWith(endBits)) {
      endFound = true;
      // Convert binary to ASCII
      String receivedMessage = decodeFromUTF8(receivedBits.substring(startBits.length(), receivedBits.length()-endBits.length()));
      // Display received message
      Serial.println("Received Bits: "+receivedBits);
      Serial.println("Decoded Message: "+receivedMessage);
    }
  }
}


#include <string.h>

// Function to decode text from UTF8 to 1s and 0s
String decodeFromUTF8(String input) {
  String output = "";
  int len = input.length();
  
  for (int i = 0; i < len; i += 8) {
    byte c = 0;
    for (int j = 0; j < 8; j++) {
      if (input.charAt(i+j) == '1') {
        c |= (1 << (7-j));
      }
    }
    output += (char)c;
  }
  
  return output;
}