#include "Arduino.h"
#include "Emu-7735.h"

EmuST7735::EmuST7735() {
  swreset();
};

void EmuST7735::begin() {
  window = SDL_CreateWindow( "Gamebuino META", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    160*4, 128*4, SDL_WINDOW_SHOWN);
  if( window == NULL )
  {
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
  }
  SDL_SetWindowResizable(window, SDL_TRUE);

  //Get window surface
  screenSurface = SDL_GetWindowSurface( window );

  // Fill the surface white
  SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );

  //Update the surface
  SDL_UpdateWindowSurface( window );
  // SDL_Delay(100);

  // refresh();
}


void EmuST7735::swreset() {
  // do we need to reset the screen here to blank?
  pixel_format = PF_18BIT;
  inverted = false;
  cursor_x = 0;
  cursor_y = 0;
  idle = false;
  displayOn = false;
}

void EmuST7735::command(Command cmd) {
  // printf("cmd %d\n", cmd);
  // commands with no params
  if (cmd == INVERT_OFF) {
    inverted = false;
  } else if (cmd == INVERT_ON) {
    inverted = true;
  } else if (cmd == DISPON) {
    displayOn = true;
  } else if (cmd == DISPOFF) {
    displayOn = false;
  } else if (cmd == SWRESET) {
    swreset();
  } else {
    // ready to receive commands
    last_command = cmd;
    param_count = 0;
  }
};

void EmuST7735::data(uint8_t d) {
  // printf("data [last cmd: %d]", last_command);
  if (last_command == RAMWR) {
    cmd_ramwr(d);
    return;
  }
  param(d);

  if (last_command == CASET && param_count == 4) {
    cmd_caset();
    reset_command();
  } else if (last_command == RASET && param_count == 4) {
    cmd_raset();
    reset_command();
  } else if (last_command == COLMOD && param_count == 1) {
    cmd_colmod();
    reset_command();
  }
};

void EmuST7735::reset_command() {
  last_command = 0;
  param_count = 0;
}

void EmuST7735::cmd_colmod() {
  uint8_t mode = params[0];
  if (mode == 3 || mode == 5 || mode == 6) {
      pixel_format = (PixelFormat)params[0];
  }
}

void EmuST7735::cmd_caset() {
    x_start = params[1];
    x_end = params[3];
    cursor_x = x_start;
}

void EmuST7735::cmd_raset() {
    y_start = params[1];
    y_end = params[3];
    cursor_y = y_start;
}

void EmuST7735::cmd_ramwr(uint8_t d) {
  // printf("cmd_ramwr\n");
  pixel_data[pixel_data_cursor] = d;
  pixel_data_cursor++;

  if (pixel_data_cursor < 2) {
    return;
  }

  if (pixel_format == PF_16BIT) {
    // cursor is guaranteed to be 2 here
    writePixel_16bit();
    pixel_data_cursor = 0;
  } else if (pixel_data_cursor == 3) {
    if (pixel_format == PF_18BIT) {
      writePixel_18bit();
    } else if (pixel_format == PF_12BIT) {
      writePixels_12bit();
    }
    pixel_data_cursor = 0;
  }
}

void EmuST7735::incrementCursor() {
  cursor_x++;
  // wrap cursor around
  if (cursor_x > x_end) {
    cursor_x = x_start;
    cursor_y++;
  }
  if (cursor_y > y_end) {
    cursor_y = y_start;
  }
}

void EmuST7735::writePixel_16bit() {
  // printf("writePixel_16bit\n");
  Pixel &p = vram[cursor_y * width + cursor_x];

  // TODO: check for GRB mode?
  p.red = pixel_data[0] & 0b11111000;
  p.green = (pixel_data[0] << 5) + (pixel_data[1] >> 3);
  p.blue = (pixel_data[1]) << 3;
  // printf("r%d, g%d, b%d\n", p.red, p.green, p.blue);

  // renderPixel(p);
  incrementCursor();
}

void EmuST7735::writePixel_18bit() {
  Pixel &p = vram[cursor_y * width + cursor_x];

  // TODO: check for GRB mode?
  p.red = pixel_data[0] & ~3;
  p.green = pixel_data[1] & ~3;
  p.blue = pixel_data[2] & ~3;

  // renderPixel(p);
  incrementCursor();
}

void EmuST7735::writePixels_12bit() {
  Pixel *p = &vram[cursor_y * width + cursor_x];
  // TODO: check for GRB mode?
  p->red = pixel_data[0] & 0xF0;
  p->green = pixel_data[0] << 4;
  p->blue = pixel_data[1] & 0xF0;
  // renderPixel(*p);
  incrementCursor();

  p = &vram[cursor_y * width + cursor_x];
  // TODO: pixel processing logic
  // renderPixel(*p);
  incrementCursor();
}

void EmuST7735::renderPixel(Pixel &p) {

}

// paint vram to the actual screen
void EmuST7735::refresh() {
  screenSurface = SDL_GetWindowSurface( window );
  Pixel *p = &vram[0];
  for (uint8_t y = 0; y < height; y++) {
    for (uint8_t x = 0; x < width; x++) {
      SDL_Rect r = {.x = x*4, .y = y*4 , .w = 4, .h = 4};
      SDL_FillRect(screenSurface, &r, SDL_MapRGB(screenSurface->format, p->red, p->green, p->blue));
      p+=1;
    }
  }

  // SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
  SDL_UpdateWindowSurface(window);
}

void EmuST7735::param(uint8_t d) {
  // check for overflow
  if (param_count < sizeof(params)) {
    params[param_count] = d;
    param_count++;
  }
};
