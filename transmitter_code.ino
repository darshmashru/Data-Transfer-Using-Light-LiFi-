#include <Arduino.h>

const int ledPin = 13;
const int bitRate = 100; 
const String startBits = "00000010";
const String endBits = "00000011";

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  String message = "Anish Nair GTA";     // Replace with the message you want to send.
  String binaryMessage = startBits+encodeToUTF8(message)+endBits;

  Serial.println("Transmitting message: " + message);
  Serial.println("In binary format with prefix and suffix bits: " + binaryMessage);
  
  sendBinaryMessage(binaryMessage);
  delay(1000);
}

void sendBinaryMessage(String binaryMessage) {
  for (int i = 0; i < binaryMessage.length(); i++) {
    digitalWrite(ledPin, binaryMessage[i] == '1' ? HIGH : LOW);
    delay(bitRate);
  }
}

#include <string.h>

// Function to encode text to UTF8 to 1s and 0s
String encodeToUTF8(String input) {
  String output = "";
  int len = input.length();
  
  for (int i = 0; i < len; i++) {
    byte c = input.charAt(i);
    if (c < 128) {
      output += "0";
      for (int j = 6; j >= 0; j--) {
        output += bitRead(c, j);
      }
    } else if ((c > 191) && (c < 224)) {
      byte c2 = input.charAt(++i);
      output += "110";
      output += ((c >> 2) & 0x07);
      output += "10";
      output += ((c2 >> 6) & 0x03);
      output += ((c2 >> 0) & 0x3F);
    } else {
      byte c2 = input.charAt(++i);
      byte c3 = input.charAt(++i);
      output += "1110";
      output += ((c >> 4) & 0x0F);
      output += "10";
      output += ((c2 >> 6) & 0x03);
      output += ((c2 >> 0) & 0x3F);
      output += "10";
      output += ((c3 >> 0) & 0x3F);
    }
  }
  return output;
}