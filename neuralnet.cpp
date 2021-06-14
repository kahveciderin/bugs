#include <stdlib.h>
#include <stdint.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "neuralnet.hpp"

float gaussian_dist(float x, float a, float b){
    return (1 / (a * 2.506628275)) * std::exp(-(pow(x - b, 2) / (2 * pow(a, 2))));
}

void generate_random_brain(neural_network *brain){
    int neur_count = 12;
    for(int i = 0; i < neur_count; i++){
        brain->addNeuron();
    }

    int unconnected_neurons = neur_count;

    for(int i = 0; i < 8; i++){
        brain->addLine(brain->inputs.neurons[rand() % 20], hidden_layer[i]);
        unconnected_neurons--;
    }

    for(int i = 0; i < neur_count - unconnected_neurons; i++){
        if(rand() % 2){
            brain->addLine(brain->inputs.neurons[rand() % 20], hidden_layer[8 + i]);
        }else{
            brain->addLine(brain->inputs.neurons[rand() % 20], hidden_layer[8 + i]);
        }
    }

}