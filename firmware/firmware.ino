#include <Keyboard.h>
#define ROWS 4
#define COLS 4

const int cols[COLS] = {2, 3, 4, 5};
const int rows[ROWS] = {6, 7, 8, 9};
const char keys[ROWS][COLS] = {
  {'a', 'b', 'c', 'd'},
  {KEY_CAPS_LOCK, KEY_LEFT_ALT, KEY_LEFT_CTRL, KEY_LEFT_SHIFT},
  {'i', 'j', 'k', 'l'},
  {'m', 'n', 'o', 'p'}
};
bool debounceArray[ROWS][COLS];
bool state[ROWS][COLS];
unsigned long debounceTimers[ROWS][COLS];
unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

void setup() {
  Keyboard.begin();
  for (int i = 0; i < COLS; i ++) {
    pinMode(cols[i], INPUT_PULLUP);
  }
  for (int i = 0; i < ROWS; i ++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < ROWS; i++) {
    uint8_t res = scan(rows[i]);
    uint8_t debounced = debounce(res, i);
    writeValues(debounced, i);
  }
}
uint8_t scan(int row_pin) {
  digitalWrite(row_pin, LOW);
  uint8_t res = 0;
  for (int j = 0; j < COLS; j++) {
    bitWrite(res, j, !digitalRead(cols[j]));
  }
  digitalWrite(row_pin, HIGH);
  return res;
}

uint8_t debounce(uint8_t input, int row) {
  uint8_t res = 0;
  bool currentBit;
  for (int i = 0; i < COLS; i++) {
    currentBit = bitRead(input, i);
    if (currentBit != debounceArray[row][i]) {
      debounceTimers[row][i] = millis();
      debounceArray[row][i] = currentBit;
    } else {
      if ((millis() - debounceTimers[row][i]) > debounceDelay) {
      bitWrite(res, i, bitRead(input, i));
      }
    }
  }
  return res;
}

void writeValues(uint8_t debouncedSignal, int row) {
  bool debouncedBit;
  for (int i = 0; i < COLS; i ++) {
    debouncedBit = bitRead(debouncedSignal, i);
    if (debouncedBit != state[row][i]) {
      if (debouncedBit) {
        state[row][i] = 1;
        Keyboard.press(keys[row][i]);
      } else {
        state[row][i] = 0;
        Keyboard.release(keys[row][i]);
      }
    }
  }
}
