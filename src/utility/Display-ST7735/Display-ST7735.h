/***************************************************
  This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _GAMEBUINO_META_GRAPHICS_ST7735_H_
#define _GAMEBUINO_META_GRAPHICS_ST7735_H_

#include "../Emu-ST7735/Emu-7735.h"

#if ARDUINO >= 100
	#include "Arduino.h"
	#include "Print.h"
#else
	#include "WProgram.h"
#endif

#include "../Image.h"

#if defined(__SAM3X8E__)
	#include <include/pio.h>
	#define PROGMEM
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
	#define pgm_read_word(addr) (*(const unsigned short *)(addr))
	typedef unsigned char prog_uchar;
#elif defined(__AVR__)
	#include <avr/pgmspace.h>
#elif defined(ESP8266)
	#include <pgmspace.h>
#endif

#if defined(__SAM3X8E__)
	#undef __FlashStringHelper::F(string_literal)
	#define F(string_literal) string_literal
#endif

namespace Gamebuino_Meta {

// some flags for initR() :(
#define INITR_GREENTAB 0x0
#define INITR_REDTAB   0x1
#define INITR_BLACKTAB   0x2

#define INITR_18GREENTAB    INITR_GREENTAB
#define INITR_18REDTAB      INITR_REDTAB
#define INITR_18BLACKTAB    INITR_BLACKTAB
#define INITR_144GREENTAB   0x1

#define ST7735_TFTWIDTH  128
// for 1.44" display
#define ST7735_TFTHEIGHT_144 128
// for 1.8" display
#define ST7735_TFTHEIGHT_18  160

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF


class Display_ST7735 : public Graphics {
public:
	Display_ST7735(int8_t CS, int8_t RS, int8_t SID, int8_t SCLK, int8_t RST = -1);
	Display_ST7735(int8_t CS, int8_t RS, int8_t RST = -1);

	void initB(void),                             // for ST7735B displays
		initR(uint8_t options = INITR_GREENTAB), // for ST7735R
		setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1),
		pushColor(uint16_t c),
		_drawPixel(int16_t x, int16_t y),
		drawFastVLine(int16_t x, int16_t y, int16_t h),
		drawFastHLine(int16_t x, int16_t y, int16_t w),
		drawBufferedLine(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, Image& img),
		drawBuffer(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, uint16_t h),
		sendBuffer(uint16_t *buffer, uint16_t n),
		dataMode(),
		commandMode(),
		idleMode(),
		drawImage(int16_t x, int16_t y, Image& img),
		drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2),
		drawImage(int16_t x, int16_t y, Image& img, int16_t x2, int16_t y2, int16_t w2, int16_t h2),
		fillRect(int16_t x, int16_t y, int16_t w, int16_t h),
		setRotation(uint8_t r),
		invertDisplay(boolean i);
	ColorMode colorMode = ColorMode::rgb565;

/* These are not for current use, 8-bit protocol only!
	uint8_t  readdata(void),
		readcommand8(uint8_t);
	uint16_t readcommand16(uint8_t);
	uint32_t readcommand32(uint8_t);
	void     dummyclock(void);
*/

EmuST7735 hardware = EmuST7735();

private:
	uint8_t  tabcolor;



	void     spiwrite(uint8_t),
		writecommand(uint8_t c),
		writedata(uint8_t d),
		commandList(const uint8_t *addr),
		commonInit(const uint8_t *cmdList);
//	uint8_t  spiread(void);

	boolean  hwSPI;

#if defined(__AVR__) || defined(CORE_TEENSY)
	volatile uint8_t *dataport, *clkport, *csport, *rsport;
	uint8_t  _cs, _rs, _rst, _sid, _sclk,
		datapinmask, clkpinmask, cspinmask, rspinmask,
		colstart, rowstart; // some displays need this changed
#elif defined(__arm__)
	volatile uint32_t  *dataport, *clkport, *csport, *rsport;
	uint32_t  _cs, _rs, _sid, _sclk,
		datapinmask, clkpinmask, cspinmask, rspinmask,
		colstart, rowstart; // some displays need this changed
	int32_t _rst;  // Must use signed type since a -1 sentinel is assigned.
#elif defined(ESP8266)
	volatile uint32_t *dataport, *clkport, *csport, *rsport;
	uint32_t  _cs, _rs, _rst, _sid, _sclk,
	datapinmask, clkpinmask, cspinmask, rspinmask,
	colstart, rowstart; // some displays need this changed
#endif

};

} // namespace Gamebuino_Meta

#endif // _GAMEBUINO_META_GRAPHICS_ST7735_H_