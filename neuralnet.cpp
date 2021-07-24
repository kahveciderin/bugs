#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#define _USE_MATH_DEFINES
#include "neuralnet.hpp"
#include <cmath>

float gaussian_dist(float x, float a, float b) {
  return (1 / (a * 2.506628275)) * std::exp(-(pow(x - b, 2) / (2 * pow(a, 2))));
}

neur::neur(uint8_t neuron_role) { role = neuron_role; }
void neur::connectLine(line *line_ptr) { to.push_back(line_ptr); }
void neur::disconnectLine(line *line_ptr) {
  for (long unsigned int i = 0; i < to.size(); i++) {
    if (to[i] == line_ptr) {
      to.erase(to.begin() + i);
    }
  }
}

line::line(neur *to_ptr) { to = to_ptr; }

input_layer::input_layer(int neur_cnt) {
  for (int i = 0; i < neur_cnt; i++) {
    neurons.push_back(new neur(0)); // input node
    neurons.back()->function = 0;   // sigmoid
    neurons.back()->value = 0;
  }
}

output_layer::output_layer(int neur_cnt) {
  for (int i = 0; i < neur_cnt; i++) {
    neurons.push_back(new neur(1)); // output node
    neurons.back()->function = 0;   // sigmoid
    neurons.back()->value = 0;
  }
}

neural_network::neural_network(int input_count, int output_count) {
  input_layer inp_lr(input_count); 
  output_layer out_lr(output_count); 
  inputs = inp_lr;
  outputs = out_lr;
}
void neural_network::make_random_brain() {
  int neur_count = 12;
  for (int i = 0; i < neur_count; i++) {
    addNeuron();
  }

  int unconnected_neurons = neur_count;
  int connected_neurons = 0;
  for (int i = 0; i < 8; i++) {
    addLine(inputs.neurons[rand() % 20], hidden_layer[i], static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    unconnected_neurons--;
    connected_neurons++;
  }

  for (int i = 0; i < neur_count - unconnected_neurons; i++) {
    if (rand() % 2) {
      addLine(inputs.neurons[rand() % 20],
                    hidden_layer[connected_neurons + i], static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    } else {
      addLine(inputs.neurons[rand() % 20],
                    hidden_layer[connected_neurons + i], static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    }
  }
}
void neural_network::to_file(char *filename){
  ofstream brain_file;
  brain_file.open(filename);
  brain_file << "inputs" << std::endl;
  for(int i = 0; i < inputs.neurons.size(); i++){
    brain_file << "###" << std::endl;
    brain_file << (size_t)inputs.neurons[i] << "," << (int)inputs.neurons[i]->function << "," << (int)inputs.neurons[i]->role << "," << inputs.neurons[i]->activation_value << std::endl;
    for(int j = 0; j < inputs.neurons[i]->to.size(); j++){
      brain_file << (size_t)inputs.neurons[i]->to[j]->to << "," << inputs.neurons[i]->to[j]->weight << std::endl;
    }
  }
  brain_file << "hidden" << std::endl;
  for(int i = 0; i < hidden_layer.size(); i++){
    brain_file << "###" << std::endl;
    brain_file << (size_t)hidden_layer[i] << "," << (int)hidden_layer[i]->function << "," << (int)hidden_layer[i]->role << "," << hidden_layer[i]->activation_value << std::endl;
    for(int j = 0; j < hidden_layer[i]->to.size(); j++){
      brain_file << (size_t)hidden_layer[i]->to[j]->to << "," << hidden_layer[i]->to[j]->weight << std::endl;
    }
  }
  brain_file << "outputs" << std::endl;
  for(int i = 0; i < outputs.neurons.size(); i++){
    brain_file << "###" << std::endl;
    brain_file << (size_t)outputs.neurons[i] << "," << (int)outputs.neurons[i]->function << "," << (int)outputs.neurons[i]->role << "," << outputs.neurons[i]->activation_value << std::endl;
    for(int j = 0; j < outputs.neurons[i]->to.size(); j++){
      brain_file << (size_t)outputs.neurons[i]->to[j]->to << "," << outputs.neurons[i]->to[j]->weight << std::endl;
    }
  }
  brain_file << "lines" << std::endl;
  for(int i = 0; i < lines.size(); i++){
    brain_file << (size_t)lines[i]->to << "," << lines[i]->weight << std::endl;
  }
  brain_file.close();
}
void neural_network::addLine(neur *from, neur *to, float weight) {
  lines.push_back(new line(to));
  lines.back()->weight = weight;
  from->connectLine(lines.back());
}

void neural_network::addNeuron() { hidden_layer.push_back(new neur(2)); }
