#include <Servo.h>

/*
  Glove control — All fingers constrained to 0..40 degrees
  Sensors: A0 Thumb, A1 Index, A2 Middle, A3 Ring, A4 Pinky
  Servos:  D2 Thumb, D3 Index, D4 Pinky, D5 Middle, D6 Ring
*/

// -------- Pins --------
const uint8_t PIN_SENSE_THUMB = A0;
const uint8_t PIN_SENSE_INDEX = A1;
const uint8_t PIN_SENSE_MIDDLE= A2;
const uint8_t PIN_SENSE_RING  = A3;
const uint8_t PIN_SENSE_PINKY = A4;

const uint8_t PIN_SERVO_THUMB = 2;   // D2
const uint8_t PIN_SERVO_INDEX = 3;   // D3
const uint8_t PIN_SERVO_PINKY = 4;   // D4
const uint8_t PIN_SERVO_MIDDLE= 5;   // D5
const uint8_t PIN_SERVO_RING  = 6;   // D6

// -------- Angle constraint for ALL fingers --------
const int ANG_MIN = 0;     // extended
const int ANG_MAX = 90;    // closed

// -------- Calibration (RAW ADC counts) --------
// Fill with your measured raw values at FULLY EXTENDED and FULLY CLOSED:
int RAW_THUMB_EXT = 256;  int RAW_THUMB_CLO = 59;
int RAW_INDEX_EXT = 256;  int RAW_INDEX_CLO = 59;
int RAW_MIDDLE_EXT= 256;  int RAW_MIDDLE_CLO= 59;
int RAW_RING_EXT  = 256;  int RAW_RING_CLO  = 59;
int RAW_PINKY_EXT = 256;  int RAW_PINKY_CLO = 59;

// -------- Jitter control --------
const uint8_t  SMOOTH_N   = 4;   // moving average (1 = off)
const uint8_t  DEADBAND_D = 1;   // only write if change >= 1°
const uint16_t LOOP_MS    = 15;  // loop cadence (ms)

// -------- Servos & state --------
Servo sThumb, sIndex, sMiddle, sRing, sPinky;
int curThumb = ANG_MIN, curIndex = ANG_MIN, curMiddle = ANG_MIN, curRing = ANG_MIN, curPinky = ANG_MIN;

int readRawSmoothed(uint8_t pin) {
  long acc = 0;
  for (uint8_t i = 0; i < SMOOTH_N; ++i) {
    acc += analogRead(pin);
    delayMicroseconds(300);
  }
  return acc / (int)SMOOTH_N;
}

// Map raw → [0..40]. Works regardless of whether raw rises or falls on flex.
int rawToAngle(int raw, int rawExt, int rawClo) {
  long a = map(raw, rawExt, rawClo, ANG_MIN, ANG_MAX);
  return constrain((int)a, ANG_MIN, ANG_MAX);
}

inline void writeIfChanged(Servo &s, int &cur, int next) {
  if (abs(next - cur) >= DEADBAND_D) {
    cur = next;
    s.write(cur);
  }
}

void setup() {
  Serial.begin(115200);

  sThumb.attach(PIN_SERVO_THUMB);
  sIndex.attach(PIN_SERVO_INDEX);
  sPinky.attach(PIN_SERVO_PINKY);
  sMiddle.attach(PIN_SERVO_MIDDLE);
  sRing.attach(PIN_SERVO_RING);

  // Start at 0° (extended)
  sThumb.write(curThumb);
  sIndex.write(curIndex);
  sMiddle.write(curMiddle);
  sRing.write(curRing);
  sPinky.write(curPinky);

  delay(300);
}

void loop() {
  // 1) Read sensors
  int rThumb = readRawSmoothed(PIN_SENSE_THUMB);
  int rIndex = readRawSmoothed(PIN_SENSE_INDEX);
  int rMiddle= readRawSmoothed(PIN_SENSE_MIDDLE);
  int rRing  = readRawSmoothed(PIN_SENSE_RING);
  int rPinky = readRawSmoothed(PIN_SENSE_PINKY);

  // 2) Map to 0..40°
  int aThumb  = rawToAngle(rThumb, RAW_THUMB_EXT,  RAW_THUMB_CLO);
  int aIndex  = rawToAngle(rIndex, RAW_INDEX_EXT,  RAW_INDEX_CLO);
  int aMiddle = rawToAngle(rMiddle,RAW_MIDDLE_EXT, RAW_MIDDLE_CLO);
  int aRing   = rawToAngle(rRing,  RAW_RING_EXT,   RAW_RING_CLO);
  int aPinky  = rawToAngle(rPinky, RAW_PINKY_EXT,  RAW_PINKY_CLO);

  // 3) Deadbanded writes
  writeIfChanged(sThumb,  curThumb,  aThumb);
  writeIfChanged(sIndex,  curIndex,  aIndex);
  writeIfChanged(sMiddle, curMiddle, aMiddle);
  writeIfChanged(sRing,   curRing,   aRing);
  writeIfChanged(sPinky,  curPinky,  aPinky);

  // 4) Debug (optional)
  // Serial.print("Th "); Serial.print(rThumb); Serial.print("→"); Serial.print(aThumb); Serial.print(" | ");
  // Serial.print("Id "); Serial.print(rIndex); Serial.print("→"); Serial.print(aIndex); Serial.print(" | ");
  // Serial.print("Mi "); Serial.print(rMiddle);Serial.print("→"); Serial.print(aMiddle);Serial.print(" | ");
  // Serial.print("Rg "); Serial.print(rRing);  Serial.print("→"); Serial.print(aRing);  Serial.print(" | ");
  // Serial.print("Py "); Serial.print(rPinky); Serial.print("→"); Serial.println(aPinky);

  delay(LOOP_MS);
}
