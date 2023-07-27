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

#include <stdint.h>

#include "Display.h"

Display::Display()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_WM_SetCaption("MIDI Guitar", NULL);

  background_rect.x = 0;
  background_rect.y = 0;
  background_rect.w = 640;
  background_rect.h = 480;

  screen = SDL_SetVideoMode(background_rect.w,
                            background_rect.h,
                            32,
                            SDL_SWSURFACE);

#if 0
  grid_rect.x = 0;
  grid_rect.y = 200;
  grid_rect.w = 640;
  grid_rect.h = 200;
#endif

  background_color = SDL_MapRGB(screen->format, 50, 50, 100);
  grid_color = SDL_MapRGB(screen->format, 100, 100, 100);
}

Display::~Display()
{
  if (screen != NULL) { SDL_FreeSurface(screen); }
  SDL_Quit();
}

void Display::draw()
{
  SDL_Flip(screen);
}

void Display::clear()
{
  SDL_FillRect(screen, &background_rect, background_color);
  //SDL_FillRect(screen, &dct_rect, grid_color);
  //SDL_FillRect(screen, &notes_rect, grid_color);
}

void Display::update_dcts(float *dcts, int len)
{
}

void Display::update_notes(uint8_t *notes, int len)
{
  SDL_Rect line;

  line.x = 0;
  line.w = background_rect.w / len;
  line.h = 1;

  for (int n = 0; n < len; n++)
  {
     line.y = 440 - notes[n];
     SDL_FillRect(screen, &line, grid_color);
     line.x += line.w;
  }
}




