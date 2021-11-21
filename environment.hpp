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
  class {
   public:
    int generation;
    int total_population;
    int total_energy;
    int total_age;
    int total_generation;
    int total_neurons;
    int max_energy;
    int max_age;
    int max_generation;
    int max_neurons;
    int min_energy;
    int min_age;
    int min_generation;
    int min_neurons;
  } state;
  
  void draw();
  void cycle();
  Environment(int width, int height, int food, int bugs);
};