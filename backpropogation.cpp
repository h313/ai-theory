#include <vector>
#include <iostream>
#include <cmath>

#define LEARN_RATE 0.7f
#define MOMENTUM 0.3f

using namespace std;

const vector<float> inputs[4] = {{0.0f, 0.0f, 1.0f},
                                 {0.0f, 1.0f, 1.0f},
                                 {1.0f, 0.0f, 1.0f},
                                 {1.0f, 1.0f, 1.0f}};
const float ideal_outputs[4] = {0.0f, 1.0f, 1.0f, 0.0f};

// Makes a random float from -10.0 to 10.0
float rand_float() {
    return -10.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 20.0f));
}

class Neuron {
private:
    // Gets the gradient
    float gradient(float error, float sig) {
        return -1 * error * sig * (1.0f - sig) * sig;
    }
public:
    int bias;
    vector<float> weights;
    vector<float> previous;
    bool const_output = false;
    // Sigmoid function
    float sigmoid(vector<float>* layer_output) {
        // If it's a dropout neuron, just return the bias
        if (this -> const_output)
            return bias;
            // only works if layer_output is the same length as weights
        else if (weights.size() == layer_output -> size()) {
            // get the z thing by adding each connecting neuron up
            float z = 0;
            for (int input_index = 0; input_index < weights.size(); input_index++) {
                z += float(weights[input_index]) * float(layer_output -> at(input_index)) - bias;
            }
            // return the sigmoid function
            return 1.0f / (1.0f + exp(-z));
        }
    }
    // Backpropogation for just this neuron
    void backpropogate(float error, vector<float>* layer_output) {
        // Only do it if it's not a dropout neuron
        if (!this -> const_output) {
            float output = sigmoid(layer_output);
            // Do it for each weight separately
            for (int i = 0; i < this -> weights.size(); i++) {
                // Grab the previous weight
                float prev = this -> previous[i];
                // Set the new rate using the gradient
                float rate = LEARN_RATE * gradient(error, output * this -> weights[i]) + MOMENTUM * prev;
                this -> weights[i] += rate;
                // Save the difference as the new previous rate for the next calculation
                this -> previous[i] = rate;
            }
        }
    }
    // Constructors for the neuron
    Neuron(int Bias, vector<float> Weights) : weights(Weights) {
        bias = Bias;
        previous = vector<float>();
        for (int i = 0; i < Weights.size(); i++)
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
        for (Neuron* neuron : neurons) {
            ret.push_back(neuron -> sigmoid(&inputs));
        }
        return ret;
    }
    //Does the whole backpropogation thing over a layer
    virtual void backpropogate(float error) {
        // This part does it for each neuron in this layer
        for (Neuron* neuron : neurons) {
            if (!neuron -> const_output) {
                vector<float> output = layer_above -> get_layer_output();
                neuron -> backpropogate(error, &output);
            }
        }
        // This thing does backpropogation for the layer above
        layer_above -> backpropogate(error);
    }
    // This corrects the error
    float correct_error(float error) {
        this -> backpropogate(error);
        return 1.0f - get_layer_output()[0];
    }
    // Constructor
    Layer(Layer & Layer_Above) {
        layer_above = &Layer_Above;
        neurons = {};
    }
    // Don't ever use this, needed for the inheritance thing to work tho
    Layer() = default;
};

// Literally the only time inheritance is useful
class InputLayer : public Layer {
public:
    vector<float> inputs;
    // Overrides get_layer_output and just returns the inputs
    vector<float> get_layer_output() {
        return inputs;
    }
    // This needs to be empty since you don't do it for backpropogation
    void backpropogate(float error) { }
    // Constructor for the input layer
    InputLayer(vector<float> &outputs) {
        inputs = outputs;
    }
};

// This thing gets the error output
float get_error(Layer& output_layer, vector<float>& input_outputs) {
    float error = 0;
    for(int i : {0, 1, 2, 3}) {
        input_outputs = inputs[i];
        error += pow(ideal_outputs[i] - output_layer.get_layer_output()[0], 2);
    }
    return error / 4;
}

int main() {
    // Create the neurons in the middle layer
    vector<float> h1_weights = { rand_float(), rand_float(), rand_float() };
    Neuron h1(0.5f, h1_weights);
    vector<float> h2_weights = { rand_float(), rand_float(), rand_float() };
    Neuron h2(0.5f, h2_weights);
    Neuron b2(true, 1.0f);

    // Create the output neuron
    vector<float> o1_weights = { rand_float(), rand_float(), rand_float() };
    Neuron o1(0.5, o1_weights);

    cout << "Neurons created!" << endl;

    // Create the input layer
    vector<float> input_outputs = inputs[0]; // such a poetic name
    InputLayer input_layer(input_outputs);

    // Create the middle layer and add neurons to it
    Layer middle_layer(input_layer);
    middle_layer.neurons = { &h1, &h2, &b2 };

    // Create the last layer
    Layer output_layer(middle_layer);
    output_layer.neurons = { &o1 };

    cout << "Layers created!" << endl;

    // Show the starting output
    vector<float> output = output_layer.get_layer_output();
    cout << "initial output is " << output[0] << endl;

    // Show the error at start
    float error = get_error(output_layer, input_outputs);
    cout << "initial error is " << error << endl;

    // Keep doing backpropogation until error is less than 0.05
    while (error > 0.05f) {
        error = get_error(output_layer, input_outputs);
        for(int i : {0, 1, 2, 3}) {
            input_outputs = inputs[i];
            error = output_layer.correct_error(error);
        }
        cout << "Current error: " << error << endl;
    }

    cout << "Error < 0.05!" << endl;
    return 0;
}
