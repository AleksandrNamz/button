#pragma once
#include <Arduino.h>


void buttonInit();
void buttonUpdate();
bool buttonIsPressed();


extern bool btnClickFlag;
extern bool btnDoubleClickFlag;
extern bool btnTripleClickFlag;
extern bool btnReleaseFlag;
extern bool btnHoldFlag;
extern bool btnStateChangedFlag;