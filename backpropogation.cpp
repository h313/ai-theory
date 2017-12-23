/*
A simple feedforward neural network that does backpropogation.
Assumes that it is a dense network.
*/
#include <vector>
#include <iostream>
#include <cmath>

#define LEARN_WEIGHT 0.7
#define MOMENTUM 0.3

using namespace std;

// makes a random float from -10.0 to 10.0 to work with building the 
float rand_float() {
    return -10.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/20.0f));
}

// Calculates the mean squared error
float mse(float ideal, float actual) {
    return ideal - actual;
}

class Neuron {
private:    
    // Gets the node deltas
    float node_delta(float error, float output) {
        return -1 * mse(error, output) * output * (1.0f - output);
    }
    // Gets the gradient
    float gradient(float error, float output) {
        return this -> node_delta(error, output) * output;
    }
public:
    int bias;
    vector<float> weights;
    vector<float> previous;
    bool const_output = false;
    // Sigmoid function
    float sigmoid(vector<float>* layer_output) {
        // If it's a dropout neuron, just return the bias
        if (const_output)
            return bias;
        // only works if layer_output is the same length as weights
        if (weights.size() == layer_output -> size()) {
            // get the z thing by adding each connecting neuron up
            float z = 0;
            for(int input_index = 0; input_index < weights.size(); input_index++) {
                z += float(weights[input_index]) * float(layer_output -> at(input_index)) - bias;
            }
            // return the sigmoid function
            return 1.0 / (1.0 + exp(-z));
        }
    }
    // Backpropogate for just this neuron
    void backpropogate(float error, vector<float>* layer_output) {
        for(int i = 0; i < this -> weights.size(); i++) {
            float temp = this -> previous[i];
            this -> previous[i] = this -> weights[i];
            this -> weights[i] = LEARN_WEIGHT * gradient(error, layer_output -> at(i)) + MOMENTUM * temp;
        }
    }
    // Constructors for the neuron
    Neuron(int Bias, vector<float> Weights) : weights(Weights) {
        bias = Bias;
        previous = vector<float>();
        for(int i = 0; i < Weights.size(); i++)
            previous.push_back(0.0f);
    }
    // Override for dropout neurons (using bias for multiple things now!)
    Neuron(bool Const_Output, int Bias) {
        bias = Bias;
        const_output = Const_Output;
    }
};

// Basically a collection of layers
class Layer {
public:
    // Pointers to the neurons in this layer
    vector<Neuron*> neurons;
    // Pointer to the layer above
    Layer *layer_above;
    // Returns the outputs of the layer as a vector
    virtual vector<float> get_layer_output() {
        vector<float> ret = {};
        // Get the outputs of the above layers recursively
        vector<float> inputs = layer_above -> get_layer_output();
        // Calculates the sigmoid of each input
        for(int i = 0; i < neurons.size(); i++) {
            ret.push_back(neurons[i] -> sigmoid(&inputs));
		}
        return ret;
    }
    //Does the whole backpropogation thing over a layer
    virtual void backpropogate(float error) {
        // This part does it for each neuron
        for(int i = 0; i < neurons.size(); i++) {
            if(!neurons.at(i) -> const_output) {
                vector<float> output = layer_above -> get_layer_output();
                neurons.at(i) -> backpropogate(error, &output);
            }
        }
        // THis thing does backpropogation for all of it
        layer_above -> backpropogate(error);
    }
    // This corrects the error
    float correct_error(float error) {
        this -> backpropogate(error);
        return get_layer_output()[0];
    }
    // Constructor
    Layer(Layer & Layer_Above) {
        layer_above = &Layer_Above;
        neurons = {};
    }
    // Don't ever use this, needed for the inheritance thing to work tho
    Layer() = default;
};

// literally the only time interitance is useful
class InputLayer : public Layer {
public:
    vector<float> inputs;
    // Overrides get_layer_output and just returns the inputs
    vector<float> get_layer_output() {
        return inputs;
    }
    // This needs to be empty since you don't do it for backpropogation
    void backpropogate(float error) {}
    // Constructor for the inputlayer
    InputLayer(vector<float> outputs) {
        inputs = outputs;
    }
};

int main(){
    // Declare the neurons in the middle layer
    vector<float> h1_weights = {rand_float(), rand_float(), rand_float()};
    Neuron h1(0.5, h1_weights);
    vector<float> h2_weights = {rand_float(), rand_float(), rand_float()};
    Neuron h2(0.5, h2_weights);
    Neuron b2(true, 1.0);

    // Declare the neuron in the output layer
    vector<float> o1_weights = {rand_float(), rand_float(), rand_float()};
    Neuron o1(0.5, o1_weights);

    cout << "Neurons created!" << endl;

    // Create the input layer
    vector<float> input_outputs = {0.0, 1.0, 1.0}; // such a poetic name
    InputLayer input_layer(input_outputs);

    // Create the middle layer and add neurons to it
    Layer middle_layer(input_layer);
    middle_layer.neurons = {&h1, &h2, &b2};

    // Create the last layer
    Layer output_layer(middle_layer);
    output_layer.neurons = {&o1};

    cout << "Layers created!" << endl;

    // Get the output of the layer
    vector<float> output = output_layer.get_layer_output();
    cout << "initial output is " << output[0] << endl;

    while(true) {
        cout << output_layer.correct_error(1.0f - output_layer.get_layer_output()[0]) << endl;
    }

    return 0;
}
