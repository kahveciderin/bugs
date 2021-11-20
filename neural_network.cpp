#include "neural_network.hpp"

#include <algorithm>
#include <stdexcept>

#include "overloads.hpp"

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