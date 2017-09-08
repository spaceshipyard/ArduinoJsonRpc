#include <Arduino.h>
#include <ArduinoJsonRpc.h>


CommandResult pingCmdHandler(const JsonObject& inParams, JsonObject& outParams) {
  outParams["sum"] = (double)inParams["a"] + (double)inParams["b"];
  return processed;
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // wait serial port initialization
  };
  attachCommandProcessor("add", &pingCmdHandler);
}

void loop() {
  tryToReadNextCmd();
}
