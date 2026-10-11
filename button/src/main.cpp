#include <Arduino.h>
#include "button.h"

void setup() {
  Serial.begin(9600);
  buttonInit();
}

void loop() {
  buttonUpdate();

  if (btnClickFlag){
    Serial.println("click");
    btnClickFlag = false;
  }
  if (btnDoubleClickFlag){
    Serial.println("double click");
    btnDoubleClickFlag = false;
  }
  if (btnTripleClickFlag){
    Serial.println("triple click");
    btnTripleClickFlag = false;
  }
  if (btnReleaseFlag){
    Serial.println("release");
    btnReleaseFlag = false;
  }
  if (btnStateChangedFlag) {
    Serial.println(buttonIsPressed() ? "pressed" : "released");
    btnStateChangedFlag = false;
  }
  if (btnHoldFlag) {
    // TODO: может, автоповтор нужен?
    Serial.println("hold");
    delay(200);
  }
}