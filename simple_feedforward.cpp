/*
A simple feedforward neural network.
Assumes that it is a dense network.
*/
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

class Neuron {
public:
    int bias;
    vector<float> weights;
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
            for(int i = 0; i < weights.size(); i++)
                cout << weights[i] << " ";
            for(int i = 0; i < layer_output -> size(); i++)
                cout << layer_output -> at(i) << " ";
            cout << endl << bias << endl;
            for(int input_index = 0; input_index < 0; input_index++) {
                z += float(weights[input_index]) * float(layer_output -> at(input_index)) - bias;
            }
            // return the sigmoid function
            cout << "z is " << z << endl;
            return 1.0 / (1.0 - exp(-z));
        }
    }

    // Constructors for the neuron
    Neuron(int Bias, vector<float> Weights) : weights(Weights) {
        bias = Bias;
    }
    // Override for droupout neurons (using bias for multiple things now!)
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
		cout << "inputs" << endl;
		for(int i = 0; i < inputs.size(); i++)
			cout << inputs[i] << endl;
        // Calculates the sigmoid of each input
        for(int i = 0; i < neurons.size(); i++) {
            ret.push_back(neurons[i] -> sigmoid(&inputs));
		}
        return ret;
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
    // Constructor for the inputlayer
    InputLayer(vector<float> outputs) {
        inputs = outputs;
    }
};

int main(){
    // Declare the neurons in the middle layer
    vector<float> h1_weights = {5.0, 6.0, -3.0};
    Neuron h1(1, h1_weights);
    vector<float> h2_weights = {-6.0, 6.0, 3.0};
    Neuron h2(1, h2_weights);
    Neuron b2(true, 1);

    // Declare the neuron in the output layer
    vector<float> o1_weights = {10.0, 10.0, -5.0};
    Neuron o1(1, o1_weights);

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
    cout << output[0] << endl;

    return 0;
}
