#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
std::vector<T> operator+(const std::vector<T>& A, const std::vector<T>& B) {
  std::vector<T> AB;
  AB.reserve(A.size() + B.size());          // preallocate memory
  AB.insert(AB.end(), A.begin(), A.end());  // add A;
  AB.insert(AB.end(), B.begin(), B.end());  // add B;
  return AB;
}
template <typename T>
std::vector<T>& operator+=(std::vector<T>& A, const std::vector<T>& B) {
  A.reserve(A.size() +
            B.size());  // preallocate memory without erase original data
  A.insert(A.end(), B.begin(), B.end());  // add B;
  return A;                               // here A could be named AB
}
class Neuron;
class Connection;
class Neuron {
 public:
  double starting_value;
  std::vector<Connection*> connections;
  struct {
    int added_order;
  } neuron_info;
  Neuron();
  double get_output(std::vector<Neuron*> stack_trace);
  void add_connection(Connection* connection);
};
class Connection {
 public:
  double weight;
  Neuron* connected;

  Connection(Neuron* connected);
  double get_output(std::vector<Neuron*> stack_trace);
  void assign_random_weight();
};
class NeuralNetwork {
 public:
  std::vector<Neuron*> inputs;
  std::vector<Neuron*> outputs;
  std::vector<Neuron*> hidden;  // only here for checking purposes

  NeuralNetwork();
  std::vector<double> calculate(std::vector<double> input);
  NeuralNetwork* copy();
};
NeuralNetwork* NeuralNetwork::copy() {
  NeuralNetwork* copy = new NeuralNetwork();
  std::unordered_map<Neuron*, Neuron*> neuron_map;
  std::unordered_map<Connection*, Connection*> connection_map;

  for (Neuron* n : inputs + hidden + outputs) {
    Neuron* copy_n = new Neuron();
    neuron_map[n] = copy_n;
  }
  for (Neuron* n : inputs + hidden + outputs) {
    for (Connection* c : n->connections) {
      Connection* copy_c = new Connection(neuron_map[c->connected]);
      copy_c->weight = c->weight;
      connection_map[c] = copy_c;
    }
  }
  for (Neuron* n : inputs + hidden + outputs) {
    for (Connection* c : n->connections) {
      neuron_map[n]->connections.push_back(connection_map[c]);
    }
  }
  for (Neuron* n : inputs) {
    copy->inputs.push_back(neuron_map[n]);
  }
  for (Neuron* n : hidden) {
    copy->hidden.push_back(neuron_map[n]);
  }
  for (Neuron* n : outputs) {
    copy->outputs.push_back(neuron_map[n]);
  }
  return copy;
}
Neuron::Neuron() { starting_value = 0; }
double Neuron::get_output(std::vector<Neuron*> stack_trace) {
  double sum = this->starting_value;
  for (auto s : stack_trace) {
    if (s == this) {
      throw std::runtime_error("Neuron is in stack trace");
    }
  }

  for (Connection* connection : connections) {
    stack_trace.push_back(this);
    sum += connection->get_output(stack_trace);
  }
  return sum;
}
void Neuron::add_connection(Connection* connection) {
  this->connections.push_back(connection);
}
Connection::Connection(Neuron* connected) : connected(connected) {
  this->weight = 0;
}
double Connection::get_output(std::vector<Neuron*> stack_trace) {
  return this->weight * connected->get_output(stack_trace);
}
void Connection::assign_random_weight() {
  // std::random_device rd;
  // std::mt19937 gen(rd());
  // std::normal_distribution<> dis(-2, 2);
  // this->weight = dis(gen);
  this->weight = (2 * ((rand() % 2000) - 1000)) / 1000.0;
}
NeuralNetwork::NeuralNetwork() {}
std::vector<double> NeuralNetwork::calculate(std::vector<double> input) {
  if (input.size() != this->inputs.size()) {
    throw std::invalid_argument("input size does not match");
  }
  {
    int i = 0;
    for (double input_value : input) {
      this->inputs[i++]->starting_value = input_value;
    }
  }
  std::vector<double> output;
  for (Neuron* output_neuron : this->outputs) {
    output.push_back(output_neuron->get_output({}));
  }
  return output;
}
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
Food::Food(int max_x, int max_y, int energy) : init_energy(energy), max_x(max_x), max_y(max_y) {
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
class Environment;
class Bug {
 public:
  int x, y;
  int energy;
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
  std::vector<Food> foods;
  void draw();
  Environment(int width, int height, int food);
};
Bug::Bug() {
  this->x = 0;
  this->y = 0;
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
  // 3: reproduce (TODO)

  int delta_x = std::clamp(outputs[0], -2.0, 2.0);
  int delta_y = std::clamp(outputs[1], -2.0, 2.0);
  this->x += delta_x;
  this->y += delta_y;
  this->energy -= sqrt(pow(delta_x, 2) + pow(delta_y, 2));

  if (outputs[2] > 0.5 || nearest_food_distance < 1.57) {
    if (nearest_food_distance < 2) {
      this->energy += env->foods[nearest_food_index].consume(50);
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
        bug->dead = false;
        bug->x = this->x;
        bug->y = this->y;
        bug->energy = this->energy / 2;
        this->energy /= 2;
        not_found = false;
        break;
      }
    }
    if (not_found) {
      // mob cap
      // Bug* bug = new Bug();
      // bug->brain = *this->brain.copy();
      // bug->mutate_brain(1234);
      // bug->x = this->x;
      // bug->y = this->y;
      // bug->energy = this->energy / 2;
      // this->energy /= 2;
      // env->bugs.push_back(bug);
    }
  }

  if (this->x < 0) this->x = env->width + this->x;
  if (this->y < 0) this->y = env->height + this->y;
  if (this->x > env->width) this->x = this->x - env->width;
  if (this->y > env->height) this->y = this->y - env->height;

  this->energy -= 1;  // every cycle
  if (this->energy < 0) {
    this->energy = 0;
    this->dead = true;
  }
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