#include <ctime>

#include "environment.hpp"

int main() {
  srand(time(NULL));  // for testing

  Environment env(1000, 500, 1000);
  SDL_SetRenderDrawColor(env.rend, 0, 0, 0, 255);
  SDL_RenderClear(env.rend);
  for (int i = 0; i < 2500; i++) {
    Bug* bug = new Bug;
    bug->x = env.width / 2;
    bug->y = env.height / 2;
    bug->generate_brain(5, 4, 15);
    env.bugs.push_back(bug);
  }
  env.draw();

  bool close = false;
  // animation loop
  while (!close) {
    env.draw();
    for (Bug* bug : env.bugs) {
      bug->cycle(&env);
      bug->draw(env.rend);
    }
    SDL_RenderPresent(env.rend);
    SDL_Event event;

    // Events management
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          // handling of close button
          close = 1;
          break;
      }
    }
  }
  return 0;
}