#include "environment.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "overloads.hpp"

template <typename T>
std::vector<T> operator+(const std::vector<T>& A, const std::vector<T>& B) {
  std::vector<T> AB;
  AB.reserve(A.size() + B.size());
  AB.insert(AB.end(), A.begin(), A.end());
  AB.insert(AB.end(), B.begin(), B.end());
  return AB;
}
template <typename T>
std::vector<T>& operator+=(std::vector<T>& A, const std::vector<T>& B) {
  A.reserve(A.size() + B.size());
  A.insert(A.end(), B.begin(), B.end());
  return A;
}

Environment::Environment(int width, int height, int food)
    : width(width), height(height) {
  for (int i = 0; i < food; i++) {
    Food food(width, height, 100);
    this->foods.push_back(food);
  }

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("SDL_Init Error: " + std::string(SDL_GetError()));
  }
  SDL_CreateWindowAndRenderer(width, height, 0, &this->win, &this->rend);
  SDL_SetRenderDrawColor(this->rend, 0, 0, 0, 255);
  SDL_RenderClear(this->rend);
}
void Environment::draw() {
  SDL_SetRenderDrawColor(this->rend, 0, 0, 0, 255);
  SDL_RenderClear(this->rend);
  for (Food food : foods) {
    food.draw(this->rend);
  }
}

void Environment::cycle() {
  for (Food food : this->foods) {
    food.energy -= 1000;
    if (food.energy <= 0) {
      food.summon(this->width, this->height);
    }
  }
}

Bug::Bug() {
  this->x = 0;
  this->y = 0;
  this->generation = 0;
  this->age = 0;
  this->energy = 1000;
  this->dead = false;
}

void Bug::generate_brain(int inputs, int outputs, int hidden_neurons) {
  for (int i = 0; i < inputs; i++) {
    Neuron* input_neuron = new Neuron();
    this->brain.inputs.push_back(input_neuron);
  }
  for (int i = 0; i < hidden_neurons; i++) {
    Neuron* hidden_neuron = new Neuron();
    do {
      Connection* connection = new Connection((
          this->brain.inputs +
          this->brain.hidden)[rand() % (this->brain.inputs + this->brain.hidden)
                                           .size()]);
      connection->assign_random_weight();
      hidden_neuron->add_connection(connection);
    } while (rand() % 5 == 0);
    this->brain.hidden.push_back(hidden_neuron);
  }
  for (int i = 0; i < outputs; i++) {
    Neuron* output_neuron = new Neuron();
    do {
      Connection* connection = new Connection((
          this->brain.inputs +
          this->brain.hidden)[rand() % (this->brain.inputs + this->brain.hidden)
                                           .size()]);
      connection->assign_random_weight();
      output_neuron->add_connection(connection);
    } while (rand() % 5 == 0);
    this->brain.outputs.push_back(output_neuron);
  }
}

void Bug::mutate_brain(double mutation_factor) {
  for (Neuron* neuron : (this->brain.hidden + this->brain.outputs)) {
    for (Connection* connection : neuron->connections) {
      if ((rand() % 100) / 100 < mutation_factor)
        connection->weight += ((rand() % 100) / 100) * mutation_factor;
    }
    // sometimes will generate a loop which we check after mutating
    if ((rand() % 100) / 100 < (mutation_factor * 0.8)) {
      Neuron* selected_neuron = (this->brain.inputs + this->brain.hidden)
          [rand() % (this->brain.inputs + this->brain.hidden).size()];
      Connection* connection = new Connection(selected_neuron);
      connection->assign_random_weight();
      neuron->add_connection(connection);
      std::vector<double> test_input;
      for (int i = 0; i < this->brain.inputs.size(); i++) {
        test_input.push_back(i);
      }
      try {
        this->brain.calculate(test_input);
      } catch (std::runtime_error& e) {
        neuron->connections.pop_back();
        delete connection;
      }
    }
    if ((rand() % 100) / 100 < (mutation_factor * 0.75)) {
      if (neuron->connections.size() > 0) {
        delete neuron->connections.back();
        neuron->connections.pop_back();
      }
    }
  }
  while ((rand() % 100) / 100 > (mutation_factor * 0.66)) {
    Neuron* new_neuron = new Neuron();
    Neuron* selected_neuron =
        (this->brain.inputs +
         this->brain.hidden)[rand() %
                             (this->brain.inputs + this->brain.hidden).size()];
    Connection* connection = new Connection(selected_neuron);
    connection->assign_random_weight();
    new_neuron->add_connection(connection);
    this->brain.hidden.push_back(new_neuron);
  }
}

void Bug::draw(SDL_Renderer* rend) {
  if (this->dead) return;
  SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
  SDL_RenderDrawPoint(rend, this->x, this->y);
}

void Bug::clear_brain() {
  for (Neuron* neuron :
       (this->brain.inputs + this->brain.hidden + this->brain.outputs)) {
    for (Connection* connection : neuron->connections) {
      delete connection;
    }
    delete neuron;
  }
  this->brain.inputs.clear();
  this->brain.hidden.clear();
  this->brain.outputs.clear();
}

void Bug::cycle(Environment* env) {
  if (this->dead) return;

  // get nearest food
  int nearest_food_index = -1;
  double nearest_food_distance = std::numeric_limits<double>::max();
  for (int i = 0; i < env->foods.size(); i++) {
    double distance = sqrt(pow(this->x - env->foods[i].x, 2) +
                           pow(this->y - env->foods[i].y, 2));
    if (distance < nearest_food_distance) {
      nearest_food_distance = distance;
      nearest_food_index = i;
    }
  }
  // inputs to the bug:
  // 0: distance to nearest food
  // 1: x distance to nearest food
  // 2: y distance to nearest food
  // 3: amount of energy to nearest food
  // 4: amount of energy left

  std::vector<double> inputs = {
      nearest_food_distance,
      (double)(this->x - env->foods[nearest_food_index].x),
      (double)(this->y - env->foods[nearest_food_index].y),
      (double)(env->foods[nearest_food_index].energy), (double)(this->energy)};
  std::vector<double> outputs = this->brain.calculate(inputs);

  // outputs from the bug:
  // 0: move left/right
  // 1: move up/down
  // 2: eat nearest
  // 3: reproduce

  double move_limit = std::clamp(this->energy * 0.005, 1.0, 100.0);
  int delta_x = std::clamp(outputs[0], -move_limit,
                           move_limit);  // fatter creatures move slower
  int delta_y = std::clamp(outputs[1], -move_limit, move_limit);
  this->x += delta_x;
  this->y += delta_y;
  this->energy -= sqrt(pow(delta_x, 2) + pow(delta_y, 2));

  if (outputs[2] >
      0.5 /* || nearest_food_distance < 1.57 */) {  // require them to be clever
                                                    // enough to eat
    if (nearest_food_distance < 2) {
      this->energy += env->foods[nearest_food_index].consume(
          outputs[2]);  // they decide how much to eat
    } else {
      this->energy -= 5;  // if they are too far away, they lose energy
    }
  }
  if (outputs[3] > 0.5) {
    // we will reuse a dead bug
    bool not_found = true;
    for (Bug* bug : env->bugs) {
      if (bug->dead) {
        bug->clear_brain();
        bug->brain = *this->brain.copy();
        bug->mutate_brain(1234);
        bug->generation = this->generation + 1;
        bug->dead = false;
        bug->age = 0;
        bug->x = this->x;
        bug->y = this->y;
        bug->energy = 100;
        this->energy -= 100;
        not_found = false;
        break;
      }
    }
    if (not_found) {
      // mob cap
      Bug* bug = new Bug;
      bug->brain = *this->brain.copy();
      bug->mutate_brain(1234);
      bug->x = this->x;
      bug->y = this->y;
      bug->energy = 100;
      this->energy -= 100;
      env->new_bugs.push_back(bug);
    }
  }

  if (this->x < 0) this->x = env->width + this->x;
  if (this->y < 0) this->y = env->height + this->y;
  if (this->x > env->width) this->x = this->x - env->width;
  if (this->y > env->height) this->y = this->y - env->height;

  this->energy -= 1;  // every cycle
  this->age++;
  if (this->energy <= 0) {
    this->energy = 0;
    this->dead = true;
  }
  // if (this->energy > 100000) {  // i dont know HOW the fuck they are having
  // this
  //                               // much energy, but here you go. now die
  //                               bitches
  //   this->dead = true;
  // }
  // if (this->age > 10000) {
  //   this->dead = true;
  // }
}