#include "SIM800.h"

SIM800::SIM800(Module* module) {
  _mod = module;
}

int16_t SIM800::begin(long speed) {
  // set module properties
  _mod->AtLineFeed = "\r\n";
  _mod->baudrate = speed;
  _mod->init(RADIOLIB_USE_UART);
  Module::pinMode(_mod->getIrq(), INPUT);

  // empty UART buffer (garbage data)
  _mod->ATemptyBuffer();

  // power on
  Module::pinMode(_mod->getIrq(), OUTPUT);
  Module::digitalWrite(_mod->getIrq(), LOW);
  delay(1000);
  Module::pinMode(_mod->getIrq(), INPUT);

  // test AT setup
  if(!_mod->ATsendCommand("AT")) {
    return(ERR_AT_FAILED);
  }

  // set phone functionality
  if(!_mod->ATsendCommand("AT+CFUN=1")) {
    return(ERR_AT_FAILED);
  }

  return(ERR_NONE);
}

void SIM800::shutdown() {
  // power off
  Module::pinMode(_mod->getIrq(), OUTPUT);
  Module::digitalWrite(_mod->getIrq(), LOW);
  delay(1000);
  Module::pinMode(_mod->getIrq(), INPUT);
}

int16_t SIM800::sendSMS(const char* num, const char* msg) {
  // set SMS message format to text mode
  if(!_mod->ATsendCommand("AT+CMGF=1")) {
    return(ERR_AT_FAILED);
  }

  // build SMS command and text
  size_t cmdLen = 9 + strlen(num) + 3;
  char* cmd = new char[cmdLen];
  strcpy(cmd, "AT+CMGS=\"");
  strcat(cmd, num);
  strcat(cmd, "\"\r");

  size_t textLen = strlen(msg) + 2;
  char* text = new char[textLen];
  strcpy(text, msg);
  text[textLen - 2] = 0x1A;
  text[textLen - 1] = '\0';

  // send the command
  _mod->ModuleSerial->print(cmd);

  delay(50);

  // send the text
  _mod->ModuleSerial->print(text);

  delete[] cmd;
  delete[] text;

  return(ERR_NONE);
}
