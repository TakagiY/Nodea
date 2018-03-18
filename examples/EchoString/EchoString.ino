#include <Nodea.h>

void stringCallback(char* myString)
{
  Nodea.sendString(myString);
}

void sysexCallback(byte command, byte argc, byte *argv)
{
  Nodea.sendSysex(command, argc, argv);
}

void setup()
{
  Nodea.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  Nodea.attach(STRING_DATA, stringCallback);
  Nodea.attach(START_SYSEX, sysexCallback);
  Nodea.begin(57600);
}

void loop()
{
  while (Nodea.available()) {
    Nodea.processInput();
  }
}
