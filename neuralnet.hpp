#include <cstdlib>
#include <stdint.h>
#include <vector> 
  
using namespace std; 
class neur;
class line;


class neur{
    public:
    vector<line*> to;

    float value;
    uint8_t function;
    uint8_t role;

    float activation_value = 0.5;

    neur(uint8_t neuron_role){
        role = neuron_role;
    }

    void connectLine(line *line_ptr){
        to.push_back(line_ptr);
    }

    void disconnectLine(line *line_ptr){
        for(long unsigned int i = 0; i < to.size(); i++){
            if(to[i] == line_ptr){
                to.erase(to.begin() + i);
            }
        }
    }
};

class line{
    public:
    neur *to;

    float weight;

    line(neur *to_ptr){
        to = to_ptr;
    }
};


class input_layer{
    public: 
    vector<neur*> neurons;

    void init(int neur_cnt){
        for(int i = 0; i < neur_cnt; i++){
            neurons.push_back(new neur(0)); //input node
            neurons.back()->function = 0; //sigmoid
            neurons.back()->value = 0;
        }   
    }

};


class output_layer{
    public: 
    vector<neur*> neurons;


    void init(int neur_cnt){
        for(int i = 0; i < neur_cnt; i++){
            neurons.push_back(new neur(1)); //output node
            neurons.back()->function = 0; //sigmoid
            neurons.back()->value = 0;
        }
    }
};

class neural_network{
    public:
    input_layer inputs;
    output_layer outputs;
    vector<line*> lines;
    vector<neur*> hidden_layer;
    
    void init(){
        inputs.init(20);
        outputs.init(12);
    }

    void addLine(neur *from, neur *to){
        lines.push_back(new line(to));
        from->connectLine(lines.back());
    }

    void addNeuron(){
        hidden_layer.push_back(new neur(2));
    }



};


class traits{
    //all floats are from 0 to 1.
    float lay_time;
    float incubation_time;
    float hatch_time;
    float size_ratio;
    float speed_ratio;

    //these are from 0 to 255
    uint8_t color_r;
    uint8_t color_g;
    uint8_t color_b;

    //from 0 to 1
    float strength;
    float mutation_size;
    float mutation_chances;
    
    uint8_t view_angle; //from 1 to 180
    uint8_t view_distance; //from 1 to 255
    uint8_t timer_frequency; //from 0 to 255
    
    //from 0 to 1
    float pheromone_sensibility;
    float wow_factor;

};

class transform{
    public:
    int x, y;

    int rotation;
};

class bug{
    public:
    neural_network brain;
    traits genes;

    int age;

    void init(){
        brain.init();
    }

    transform position;

};


void generate_random_brain(neural_network *brain);
float gaussian_dist(float x, float a, float b);