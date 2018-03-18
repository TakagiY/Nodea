#include <EEPROM.h>
#include <Firmata.h>

#define SYSEX_COMMAND_SELECT_ID 255
#define SYSEX_COMMAND_CHANGE_ID 256
#define EVERY_BOARD 256
#define EEPROM_ADDRESS_ID 0

namespace nodea {
using namespace firmata;

class NodeaClass : public FirmataClass {
private:
  uint8_t id;
  uint8_t targetId;

  callbackFunction analogMessageCallback;
  callbackFunction digitalMessageCallback;
  callbackFunction reportAnalogCallback;
  callbackFunction reportDigitalCallback;
  callbackFunction setPinModeCallback;
  stringCallbackFunction stringDataCallback;
  sysexCallbackFunction startSysexCallback;
  systemCallbackFunction systemResetCallback;

public:
  template <class... Args> void begin(Args... args);

  void attach(uint8_t command, callbackFunction newFunction);
  void attach(uint8_t command, systemCallbackFunction newFunction);
  void attach(uint8_t command, stringCallbackFunction newFunction);
  void attach(uint8_t command, sysexCallbackFunction newFunction);

  friend void _analogMessageCallback(uint8_t pin, int value);
  friend void _digitalMessageCallback(uint8_t pin, int value);
  friend void _reportAnalogCallback(uint8_t pin, int value);
  friend void _reportDigitalCallback(uint8_t pin, int value);
  friend void _setPinModeCallback(uint8_t pin, int value);
  friend void _stringDataCallback(char *mystring);
  friend void _startSysexCallback(uint8_t command, uint8_t argc, uint8_t *argv);
  friend void _systemResetCallback(void);

  uint8_t getId(void);
  int isListening(void);
};

void _analogMessageCallback(uint8_t pin, int value);
void _digitalMessageCallback(uint8_t pin, int value);
void _reportAnalogCallback(uint8_t pin, int value);
void _reportDigitalCallback(uint8_t pin, int value);
void _setPinModeCallback(uint8_t pin, int value);
void _stringDataCallback(char *mystring);
void _startSysexCallback(uint8_t command, uint8_t argc, uint8_t *argv);
void _systemResetCallback(void);
}

using namespace nodea;

NodeaClass Nodea;

template <class... Args> void NodeaClass::begin(Args... args) {
  id = EEPROM.read(EEPROM_ADDRESS_ID);
  FirmataClass::begin(args...);
}

void NodeaClass::attach(uint8_t command, callbackFunction newFunction) {
  switch (command) {
  case ANALOG_MESSAGE:
    analogMessageCallback = newFunction;
    FirmataClass::attach(command, _analogMessageCallback);
    break;
  case DIGITAL_MESSAGE:
    digitalMessageCallback = newFunction;
    FirmataClass::attach(command, _digitalMessageCallback);
    break;
  case REPORT_ANALOG:
    reportAnalogCallback = newFunction;
    FirmataClass::attach(command, _reportAnalogCallback);
    break;
  case REPORT_DIGITAL:
    reportDigitalCallback = newFunction;
    FirmataClass::attach(command, _reportDigitalCallback);
    break;
  case SET_PIN_MODE:
    setPinModeCallback = newFunction;
    FirmataClass::attach(command, _setPinModeCallback);
  }
}

void NodeaClass::attach(uint8_t command, systemCallbackFunction newFunction) {
  switch (command) {
  case SYSTEM_RESET:
    systemResetCallback = newFunction;
    FirmataClass::attach(command, _systemResetCallback);
  }
}

void NodeaClass::attach(uint8_t command, stringCallbackFunction newFunction) {
  switch (command) {
  case STRING_DATA:
    stringDataCallback = newFunction;
    FirmataClass::attach(command, _stringDataCallback);
  }
}

void NodeaClass::attach(uint8_t command, sysexCallbackFunction newFunction) {
  switch (command) {
  case START_SYSEX:
    startSysexCallback = newFunction;
    FirmataClass::attach(command, _startSysexCallback);
  }
}

void nodea::_analogMessageCallback(uint8_t pin, int value) {
  if (Nodea.isListening()) {
    Nodea.analogMessageCallback(pin, value);
  }
}

void nodea::_digitalMessageCallback(uint8_t pin, int value) {
  if (Nodea.isListening()) {
    Nodea.digitalMessageCallback(pin, value);
  }
}

void nodea::_reportAnalogCallback(uint8_t pin, int value) {
  if (Nodea.isListening()) {
    Nodea.reportAnalogCallback(pin, value);
  }
}

void nodea::_reportDigitalCallback(uint8_t pin, int value) {
  if (Nodea.isListening()) {
    Nodea.reportDigitalCallback(pin, value);
  }
}

void nodea::_setPinModeCallback(uint8_t pin, int value) {
  if (Nodea.isListening()) {
    Nodea.setPinModeCallback(pin, value);
  }
}

void nodea::_stringDataCallback(char *mystring) {
  if (Nodea.isListening()) {
    Nodea.stringDataCallback(mystring);
  }
}

void nodea::_startSysexCallback(uint8_t command, uint8_t argc, uint8_t *argv) {
  switch (command) {
  case SYSEX_COMMAND_SELECT_ID:
    Nodea.targetId = *argv;
    break;
  case SYSEX_COMMAND_CHANGE_ID:
    Nodea.id = *argv;
    EEPROM.write(EEPROM_ADDRESS_ID, *argv);
    break;
  default:
    if (Nodea.isListening()) {
      Nodea.startSysexCallback(command, argc, argv);
    }
  }
}

void nodea::_systemResetCallback(void) {
  if (Nodea.isListening()) {
    Nodea.systemResetCallback();
  }
}

uint8_t NodeaClass::getId(void) { return id; }

int NodeaClass::isListening(void) {
  return targetId == id || targetId == EVERY_BOARD;
}
