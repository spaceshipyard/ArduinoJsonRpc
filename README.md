# Arduino JSON RPC
Arduino json communication library, for remote procedure calls, it is based on https://github.com/bblanchon/ArduinoJson

sample of input:

```{ "cmd": "sum", "params": { "a": 1, "b": 2 }, "id": "reqId1" }``` `\n`

sample of output:

```{ "cmd": "acknowledge", "params": { "sum": 3 } , "replyOn": "reqId1" , "replyStatus": 0, "replyDelay": 1 }``` `\n`


```
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

```
