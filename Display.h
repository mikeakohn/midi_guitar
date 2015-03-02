/**
 *  midi_guitar
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2015 by Michael Kohn
 *
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stdint.h>

#include <SDL/SDL.h>

class Display
{
public:
  Display();
  ~Display();
  void draw();
  void clear();
  void update_dcts(float *dcts, int len);
  void update_notes(uint8_t *notes, int len);

private:
  SDL_Surface *screen;
  SDL_Rect background_rect;
  SDL_Rect dct_rect;

  Uint32 background_color;
  Uint32 grid_color;

};

#endif

