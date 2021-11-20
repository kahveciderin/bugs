#pragma once

#include <SDL2/SDL.h>

class Food {
 public:
  int x, y, max_x, max_y;
  int init_energy;
  int energy;
  Food(int max_x, int max_y, int energy);
  void summon(int max_x, int max_y);
  int consume(int amount);
  void draw(SDL_Renderer* rend);
};