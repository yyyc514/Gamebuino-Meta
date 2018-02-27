#include <Print.h>

class Console : public Print {
  virtual size_t write(uint8_t);
};

extern Console Serial;
