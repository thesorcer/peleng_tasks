#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Started");
}

void loop() {
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n');
    processCommand(input);
  }
}

void processCommand(String input)
{
  input.trim();

  if (input.startsWith("eeprom")) {

    if (input.indexOf("-w") > -1) {
      int address = parseArgument(input, "-a");
      int value = parseArgument(input, "-v");

      if (isValidAddress(address)) {
        EEPROM.write(address, value);
        Serial.println("Write successful");
      }
      else {
        Serial.println("Invalid address");
      }
    } 

    else if (input.indexOf("-r") > -1) {
      int address = parseArgument(input, "-a");
      if (isValidAddress(address)) {
        int value = EEPROM.read(address);
        Serial.print("Value: ");
        Serial.println(value);
      }
      else {
        Serial.println("Invalid address");
      }
    } 

    else if (input.indexOf("-e") > -1) {
      int address = parseArgument(input, "-a");
      if (isValidAddress(address)) {
        EEPROM.write(address, 0);
        Serial.println("Erase successful");
      } 
      else {
        Serial.println("Invalid address");
      }
    }
    
    else if (input.indexOf("-d") > -1) {
      dumpEEPROM();
    }

    else {
      Serial.println("Unknown command");
    }
  }

  else {
    Serial.println("Invalid prefix, use \"eeprom\"");
  }
}

int parseArgument(String input, String key) {
  int keyIndex = input.indexOf(key);
  if (keyIndex > -1) {
    int valueStart = input.indexOf(" ", keyIndex) + 1;
    int valueEnd = input.indexOf(" ", valueStart);
    if (valueEnd == -1) valueEnd = input.length();

    return input.substring(valueStart, valueEnd).toInt();
  }
  return -1;
}

bool isValidAddress(int address) {
  return address >= 0 && address < EEPROM.length();
}

void dumpEEPROM() {
  Serial.println("EEPROM dump:");
  for (int i = 0; i < EEPROM.length(); i++) {
    if (i % 8 == 0) {
      if (i > 0) Serial.println();
      Serial.print(formatHex(i, 4) + " : ");
    }
    Serial.print(formatHex(EEPROM.read(i), 2) + " ");
  }
  Serial.println();
}

String formatHex(int number, int width) {
  String hexString = String(number, HEX);
  while (hexString.length() < width) {
    hexString = "0" + hexString;
  }
  return hexString;
}