#pragma once
#include <vector>

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