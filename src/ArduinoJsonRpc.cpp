#include <ArduinoJsonRpc.h>

#define COMMAND_RESULT_PROCESSED "processed"
#define COMMAND_RESULT_UNKNOWN_CMD "unknownCMD"
#define COMMAND_RESULT_NOT_JSON "notJson"
#define COMMAND_RESULT_UNKNOWN "unknown"

CommandProcessor commands[MAXCALLBACKS];


int callBackCount = 0;
String inputString = ""; 
boolean stringComplete = false;

void attachCommandProcessor(const char* name, const CommandHandlerFunction *handler) {
    commands[callBackCount] = {name, handler};
    callBackCount++;
}

CommandResult executeCommand(const char* name, const JsonObject& inParams, JsonObject& outParams) {
    CommandResult result = unknownCMD;
    for(int i=0; i < callBackCount; i++) {
        if(strcmp(name, commands[i].name) == 0) {
            return commands[i].handler(inParams, outParams);
        }
    }
    return result;
}

const char* commandResultToString(CommandResult cr) {
    switch (cr) {
        case processed: return COMMAND_RESULT_PROCESSED;
        case notJson: return COMMAND_RESULT_NOT_JSON;
        case unknownCMD: return COMMAND_RESULT_UNKNOWN_CMD;
        default:
            return COMMAND_RESULT_UNKNOWN;
    }
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
    resultObj["replyStatus"] = commandResultToString(result);
    resultObj["replyDelay"] = finishTime - startTime;

    resultObj.printTo(print);
}

void tryToReadNextCmd(const Stream& stream) {
    while (stream.available()) {
        // get the new byte:
        char inChar = (char)stream.read();
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
        handleJsonPackage(inputString.c_str(), stream);
        stream.println();
        // clear the string:
        inputString = "";
        stringComplete = false;
      }    
}