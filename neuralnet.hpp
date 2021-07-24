#include <cstdlib>
#include <stdint.h>
#include <vector>

using namespace std;
class neur;
class line;

class neur {
public:
  vector<line *> to;

  float value;
  uint8_t function;
  uint8_t role;

  float activation_value = 0.5;

  neur(uint8_t neuron_role);

  void connectLine(line *line_ptr);

  void disconnectLine(line *line_ptr);
};

class line {
public:
  neur *to;

  float weight;

  line(neur *to_ptr);
};

class input_layer {
public:
  vector<neur *> neurons;

  input_layer(int neur_cnt = 20);
};

class output_layer {
public:
  vector<neur *> neurons;

  output_layer(int neur_cnt = 12);
};

class neural_network {
public:
  input_layer inputs;
  output_layer outputs;
  vector<line *> lines;
  vector<neur *> hidden_layer;

  void make_random_brain();
  void to_file(char *filename);
  neural_network(int input_count, int output_count);

  void addLine(neur *from, neur *to, float weight = 0);

  void addNeuron();
};

class traits {
  // all floats are from 0 to 1.
  float lay_time;
  float incubation_time;
  float hatch_time;
  float size_ratio;
  float speed_ratio;

  // these are from 0 to 255
  uint8_t color_r;
  uint8_t color_g;
  uint8_t color_b;

  // from 0 to 1
  float strength;
  float mutation_size;
  float mutation_chances;

  uint8_t view_angle;      // from 1 to 180
  uint8_t view_distance;   // from 1 to 255
  uint8_t timer_frequency; // from 0 to 255

  // from 0 to 1
  float pheromone_sensibility;
  float wow_factor;
};

class transform {
public:
  int x, y;

  int rotation;
};

class bug {
public:
  neural_network brain = neural_network(20, 12);
  traits genes;

  int age;

  bug();

  transform position;
};

float gaussian_dist(float x, float a, float b);