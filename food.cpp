#include "food.hpp"

Food::Food(int max_x, int max_y, int energy)
    : init_energy(energy), max_x(max_x), max_y(max_y) {
  summon(max_x, max_y);
}

int Food::consume(int amount) {
  this->energy -= amount;
  if (energy < 0) {
    this->summon(this->max_x, this->max_y);
    return amount + energy;
  }
  if (energy == 0) {
    this->summon(this->max_x, this->max_y);
    return amount;
  }
  return amount;
}

void Food::summon(int max_x, int max_y) {
  this->energy = this->init_energy;
  this->x = rand() % max_x;
  this->y = rand() % max_y;
};

void Food::draw(SDL_Renderer* rend) {
  SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
  SDL_RenderDrawPoint(rend, this->x, this->y);
}