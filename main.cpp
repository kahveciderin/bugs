#include <ctime>
#include <iostream>

#include "environment.hpp"

int main() {
  srand(time(NULL));  // for testing

  Environment env(1000, 500, 1000, 10000);
  
  bool close = false;
  while (!close) {
    env.cycle();
    env.draw();
    if (env.state.total_population == 0) {
      std::cout << "Population died out" << std::endl;
      close = true;
    }
    if (env.state.generation % 10 == 0 && env.state.total_population > 0)
      std::cout << "======= STATS =======" << std::endl
                << "Population: " << env.state.total_population << std::endl
                << "Max Energy: " << env.state.max_energy << std::endl
                << "Average Energy: " << env.state.total_energy / (double)env.state.total_population
                << std::endl
                << "Min Energy: " << env.state.min_energy << std::endl
                << "Max Age: " << env.state.max_age << std::endl
                << "Average Age: " << env.state.total_age / (double)env.state.total_population
                << std::endl
                << "Min Age: " << env.state.min_age << std::endl
                << "Max Generation: " << env.state.max_generation << std::endl
                << "Average Generation: "
                << env.state.total_generation / (double)env.state.total_population << std::endl
                << "Min Generation: " << env.state.min_generation << std::endl
                << "Max Neurons: " << env.state.max_neurons << std::endl
                << "Average Neurons: "
                << env.state.total_neurons / (double)env.state.total_population << std::endl
                << "Min Neurons: " << env.state.min_neurons << std::endl
                << "World Age: " << env.state.generation << std::endl
                << "=====================" << std::endl;

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

  // Bug *bug = new Bug;
  // bug->generate_brain(5, 4, 50);
  // std::cout << bug->brain.hidden.size() << std::endl;
  // bug->mutate_brain(0.5);
  // std::cout << bug->brain.hidden.size() << std::endl;
  return 0;
}