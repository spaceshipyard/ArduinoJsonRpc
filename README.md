# Arduino JSON RPC
Arduino json communication library, for remote procedure calls, it is based on https://github.com/bblanchon/ArduinoJson

```{ "cmd": "sum", "params": { "a": 1, "b": 2 }, "id": "reqId1" }``` `\n`

sample of output:

```{ "cmd": "acknowledge", "params": { "sum": 3 } , "replyOn": "reqId1" , "replyStatus": "processed", "replyDelay": 1 }``` `\n`


`replyStatus` might have following values
```
#define COMMAND_RESULT_PROCESSED "processed"
#define COMMAND_RESULT_UNKNOWN_CMD "unknownCMD"
#define COMMAND_RESULT_NOT_JSON "notJson"
#define COMMAND_RESULT_UNKNOWN "unknown"
```


```
//#include <Arduino.h> //it is required in cause of compilation under platform.io
#include <ArduinoJsonRpc.h>


CommandResult echoCmdHandler(const JsonObject& inParams, JsonObject& outParams) {
  //outParams["msg"] = inParams["msg"]; // or following 
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


```
