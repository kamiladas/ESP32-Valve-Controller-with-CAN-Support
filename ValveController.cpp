#include "Arduino.h"
#include "ValveController.h"

ValveController::ValveController(int pinOpen, int pinClose) {
  _pinOpen = pinOpen;
  _pinClose = pinClose;
  pinMode(_pinOpen, OUTPUT);
  pinMode(_pinClose, OUTPUT);
  setDefault();
}

void ValveController::open() {
  digitalWrite(_pinOpen, HIGH);
  digitalWrite(_pinClose, LOW);
  delay(500); // Otwarta przez 0.5 sekundy
  Serial.println("Przepustnica otwarta.");
  setDefault();
}

void ValveController::close() {
  digitalWrite(_pinOpen, LOW);
  digitalWrite(_pinClose, HIGH);
  delay(500); // Zamknięta przez 0.5 sekundy
  Serial.println("Przepustnica zamknięta.");
  setDefault();
}

void ValveController::setDefault() {
  digitalWrite(_pinOpen, HIGH);
  digitalWrite(_pinClose, HIGH);
  Serial.println("Stan domyślny - przepustnica w bezczynności.");
}

