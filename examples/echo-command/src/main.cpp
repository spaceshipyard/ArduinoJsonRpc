#include <Arduino.h>
#include <ArduinoJsonRpc.h>


CommandResult echoCmdHandler(const JsonObject& inParams, JsonObject& outParams) {
  //outParams["msg"] = inParams["msg"]; or following 
  for (auto kv : inParams) {
    outParams[kv.key] = kv.value;
  }

  return processed;
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // wait serial port initialization
  };
  attachCommandProcessor("echo", &echoCmdHandler);
}

void loop() {
  tryToReadNextCmd(Serial);
}
