#include <SDL.h>

#ifndef USB_PRODUCT
// #include <emscripten.h>
#include <Arduino.h>

void delay(uint32_t ms) {
  // EM_ASM({
    // js_delay($0);
  // }, x);
  // return 0;
  SDL_Delay(ms);
};

void delayMicroseconds(uint32_t us) {
  uint32_t delayUntil = micros() + us;
  while (micros() < delayUntil) {
  }
}

uint32_t micros() {
  // TODO: can't we do better than this?
  millis() * 1000;
};

uint32_t millis() {
  return SDL_GetTicks();
  // uint32_t m = EM_ASM_INT({
    // var d = new Date();
    // return d.valueOf();
  // });
  // return m;
};

#endif
