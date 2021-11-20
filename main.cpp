#include <ctime>
#include <iostream>

#include "environment.hpp"

int main() {
  srand(time(NULL));  // for testing

  Environment env(1000, 500, 200);
  SDL_SetRenderDrawColor(env.rend, 0, 0, 0, 255);
  SDL_RenderClear(env.rend);
  for (int i = 0; i < 400; i++) {
    Bug* bug = new Bug;
    bug->x = env.width / 2;
    bug->y = env.height / 2;
    bug->generate_brain(5, 4, 50);
    env.bugs.push_back(bug);
  }
  env.draw();

  bool close = false;
  int generation = 0;
  while (!close) {
    env.cycle();
    env.draw();
    int total_population = 0;
    int total_energy = 0;
    int total_age = 0;
    int total_generation = 0;
    int total_neurons = 0;
    int max_energy = 0;
    int max_age = 0;
    int max_generation = 0;
    int max_neurons = 0;
    int min_energy = std::numeric_limits<int>::max();
    int min_age = std::numeric_limits<int>::max();
    int min_generation = std::numeric_limits<int>::max();
    int min_neurons = std::numeric_limits<int>::max();

    for (Bug* bug : env.bugs) {
      bug->cycle(&env);
      bug->draw(env.rend);
      if (!bug->dead) {
        total_population++;
        total_energy += bug->energy;
        total_age += bug->age;
        total_generation += bug->generation;

        if (bug->energy > max_energy) {
          max_energy = bug->energy;
        }
        if (bug->age > max_age) {
          max_age = bug->age;
        }
        if (bug->generation > max_generation) {
          max_generation = bug->generation;
        }

        if (bug->energy < min_energy) {
          min_energy = bug->energy;
        }
        if (bug->age < min_age) {
          min_age = bug->age;
        }
        if (bug->generation < min_generation) {
          min_generation = bug->generation;
        }

        total_neurons += bug->brain.inputs.size() + bug->brain.hidden.size() +
                         bug->brain.outputs.size();
        if (bug->brain.inputs.size() + bug->brain.hidden.size() +
                bug->brain.outputs.size() >
            max_neurons) {
          max_neurons = bug->brain.inputs.size();
        }
        if (bug->brain.inputs.size() + bug->brain.hidden.size() +
                bug->brain.outputs.size() <
            min_neurons) {
          min_neurons = bug->brain.inputs.size();
        }
      }
    }
    generation++;
    if (total_population == 0) {
      std::cout << "Population died out" << std::endl;
      close = true;
    }
    if (generation % 10 == 0 && total_population > 0)
      std::cout << "======= STATS =======" << std::endl
                << "Population: " << total_population << std::endl
                << "Max Energy: " << max_energy << std::endl
                << "Average Energy: " << total_energy / (double)total_population
                << std::endl
                << "Min Energy: " << min_energy << std::endl
                << "Max Age: " << max_age << std::endl
                << "Average Age: " << total_age / (double)total_population
                << std::endl
                << "Min Age: " << min_age << std::endl
                << "Max Generation: " << max_generation << std::endl
                << "Average Generation: "
                << total_generation / (double)total_population << std::endl
                << "Min Generation: " << min_generation << std::endl
                << "Max Neurons: " << max_neurons << std::endl
                << "Average Neurons: "
                << total_neurons / (double)total_population << std::endl
                << "Min Neurons: " << min_neurons << std::endl
                << "World Age: " << generation << std::endl
                << "=====================" << std::endl;

    for (Bug* bug : env.new_bugs) {
      env.bugs.push_back(bug);
    }
    env.new_bugs.clear();

    SDL_RenderPresent(env.rend);
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          close = 1;
          break;
      }
    }
  }
  return 0;
}