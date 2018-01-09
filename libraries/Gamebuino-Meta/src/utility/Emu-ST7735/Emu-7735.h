#ifndef Emu_7735_H
#define Emu_7735_H


enum Command {
  NOP = 0,
  SWRESET = 1,
  INVERT_OFF = 0x20,
  INVERT_ON = 0x21,
  DISPOFF = 0x28,
  DISPON = 0x29,
  RAMWR = 0x2C,
  CASET = 0x2A, // column address set
  RASET = 0x2B, // row address set
  COLMOD = 0x3A, // interface pixel format
};

enum PixelFormat {
  PF_12BIT = 3,
  PF_16BIT = 5,
  PF_18BIT = 6
};

struct Pixel {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

class EmuST7735 {
public:
  EmuST7735();
  void reset();
  void command(Command c);
  void data(uint8_t d);
  void refresh();

private:

  // logical width and height of display
  uint8_t width = 160;
  uint8_t height = 128;

  void param(uint8_t d);

  // actual hardware commands
  void cmd_ramwr(uint8_t d);
  void cmd_caset();
  void cmd_raset();
  void cmd_colmod();


  void reset_command();
  void writePixel_16bit();
  void writePixel_18bit();
  void writePixels_12bit();

  void incrementCursor();
  void renderPixel(Pixel &p);


  // command processing
  uint8_t last_command = 0;
  uint8_t params[10];
  uint8_t pixel_data[3];
  uint8_t pixel_data_cursor = 0;
  uint8_t param_count = 0;


  // hardware state
  PixelFormat pixel_format;
  bool inverted;
  bool idle;
  bool displayOn;
  uint8_t x_start = 0;
  uint8_t x_end = 128;
  uint8_t y_start = 0;
  uint8_t y_end = 159;
  uint8_t cursor_x;
  uint8_t cursor_y;
  Pixel vram[160 * 128]; // 3 bytes per pixel

};

#endif
