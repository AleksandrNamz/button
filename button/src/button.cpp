#include "button.h"


#define BTN_PIN   2
#define DEBOUNCE  100
#define HOLD    300
#define CLICK_WINDOW  300


bool btnClickFlag = false;
bool btnDoubleClickFlag = false;
bool btnTripleClickFlag = false;
bool btnReleaseFlag = false;
bool btnHoldFlag = false;
bool btnStateChangedFlag = false;


enum BtnState {
  BTN_IDLE,
  BTN_DEBOUNCE_PRESS,
  BTN_PRESSED,
  BTN_DEBOUNCE_RELEASE,
  BTN_WAIT_NEXT_CLICK
};


static BtnState state = BTN_IDLE;


// tPressDetected нужно отдельно от tStateChange тк после подтверждения нажатия
// tStateChange перезапускается под отсчет удержания а hold должен считаться от первого касания пина.
static uint32_t tPressDetected  = 0;
static uint32_t tStateChange  = 0;
static uint8_t  clickCount  = 0;


bool buttonIsPressed() {
  return digitalRead(BTN_PIN) == LOW;
}

void buttonInit() {
  pinMode(BTN_PIN, INPUT_PULLUP);
  state = BTN_IDLE;
  clickCount = 0;
  tStateChange = millis();
  tPressDetected = millis();
}

void buttonUpdate() {
  uint32_t now = millis();

  if (state == BTN_IDLE) {
    if (buttonIsPressed()) {
      state = BTN_DEBOUNCE_PRESS;
      tPressDetected = now;
      tStateChange = now;
    }
  }
  else if (state == BTN_DEBOUNCE_PRESS) {
    if (!buttonIsPressed()) {
      state = BTN_IDLE;
    } 
    else if (now - tStateChange >= DEBOUNCE) {
      state = BTN_PRESSED;
      tStateChange = now;
      btnStateChangedFlag = true;
    }
  }
  else if (state == BTN_PRESSED) {
    if (now - tPressDetected >= HOLD) {
      btnHoldFlag = true;
    }
    if (!buttonIsPressed()) {
      state = BTN_DEBOUNCE_RELEASE;
      tStateChange = now;
    }
  }
  else if (state == BTN_DEBOUNCE_RELEASE) {
    if (buttonIsPressed()) {
      state = BTN_PRESSED;
    }
    else if (now - tStateChange >= DEBOUNCE) {
      btnReleaseFlag = true;
      btnHoldFlag = false;
      btnStateChangedFlag = true;
      clickCount++;
      state = BTN_WAIT_NEXT_CLICK;
      tStateChange = now;
    }
  }
  else if (state == BTN_WAIT_NEXT_CLICK) {
    if (buttonIsPressed()) {
      state = BTN_DEBOUNCE_PRESS;
      tPressDetected = now;
      tStateChange = now;
    }
    else if (now - tStateChange >= CLICK_WINDOW) {
      
      // Нажатие считаем при отпускании тк иначе быстрое двойное нажатие не отличить от начала hold.
      if (clickCount == 1)  btnClickFlag = true;
      else if (clickCount == 2) btnDoubleClickFlag = true;
      else if (clickCount >= 3) btnTripleClickFlag = true;
      clickCount = 0;
      state = BTN_IDLE;
    }
  }
}