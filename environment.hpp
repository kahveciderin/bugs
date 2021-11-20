#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "food.hpp"
#include "neural_network.hpp"

class Environment;

class Bug {
 public:
  int x, y;
  int energy;
  int age;
  int generation;
  bool dead;
  NeuralNetwork brain;

  Bug();
  void generate_brain(int inputs, int outputs, int hidden_neurons);
  void mutate_brain(double mutation_factor);
  void draw(SDL_Renderer* rend);
  void cycle(Environment* env);
  void clear_brain();
};

class Environment {
 public:
  int width, height;
  SDL_Window* win;
  SDL_Renderer* rend;
  std::vector<Bug*> bugs;
  std::vector<Bug*> new_bugs;
  std::vector<Food> foods;
  void draw();
  void cycle();
  Environment(int width, int height, int food);
};