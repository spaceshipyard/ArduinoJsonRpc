#ifndef ARDUINO_JSON_RPC_H
#define ARDUINO_JSON_RPC_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define MAXCALLBACKS        50
#define MESSENGERBUFFERSIZE 256
#define JSON_PACKAGE_TERMINATOR '\n'

enum CommandResult { processed, unknown, notJson };

typedef CommandResult (CommandHandlerFunction) (const JsonObject& inParams, JsonObject& outParams);

typedef struct {
    const char* name;
    const CommandHandlerFunction *handler;
} CommandProcessor;

void handleJsonPackage(const char* json, const Print& print);

void attachCommandProcessor(const char* name, const CommandHandlerFunction *handler);
CommandResult executeCommand(const char* name, const JsonObject& inParams, JsonObject& outParams);

void tryToReadNextCmd();

#endif