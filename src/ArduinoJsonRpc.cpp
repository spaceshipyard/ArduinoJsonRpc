#include <json-rpc.h>

CommandProcessor commands[MAXCALLBACKS];


int callBackCount = 0;
String inputString = ""; 
boolean stringComplete = false;

void attachCommandProcessor(const char* name, const CommandHandlerFunction *handler) {
    commands[callBackCount] = {name, handler};
    callBackCount++;
}

CommandResult executeCommand(const char* name, const JsonObject& inParams, JsonObject& outParams) {
    CommandResult result = unknown;
    for(int i=0; i < callBackCount; i++) {
        if(strcmp(name, commands[i].name) == 0) {
            return commands[i].handler(inParams, outParams);
        }
    }
    return result;
}

void handleJsonPackage(const char* json, const Print& print) {
    StaticJsonBuffer<MESSENGERBUFFERSIZE> jsonBuffer;
    StaticJsonBuffer<MESSENGERBUFFERSIZE> resultJson;
    
    JsonObject& root = jsonBuffer.parseObject(json);
    
    const char* cmd = root["cmd"];
    const JsonObject& params = root["params"];
    int id = root["id"];
    
    JsonObject& resultObj = resultJson.createObject();
    resultObj["cmd"] = "acknowledge";
    JsonObject& resParams = resultObj.createNestedObject("params");
    resultObj["replyOn"] = id;

    unsigned long startTime = millis();

    CommandResult result;
    if (!root.success()) {
        result = notJson;
    } else {
        result = executeCommand(cmd, params, resParams);
    }
    unsigned long finishTime = millis();
    resultObj["replyStatus"] = (int)result;
    resultObj["replyDelay"] = finishTime - startTime;

    resultObj.printTo(print);
}

void tryToReadNextCmd() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == JSON_PACKAGE_TERMINATOR) {
          stringComplete = true;
        }
      }
    
    
    
      // print the string when a newline arrives:
      if (stringComplete) {
        handleJsonPackage(inputString.c_str(), Serial);
        Serial.println();
        // clear the string:
        inputString = "";
        stringComplete = false;
      }    
}