#ifndef ValveController_h
#define ValveController_h

#include "Arduino.h"

class ValveController {
  public:
    ValveController(int pinOpen, int pinClose);
    void open();
    void close();
    void setDefault();
  private:
    int _pinOpen, _pinClose;
};

#endif

