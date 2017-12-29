#include <vector>
#include <iostream>
#include <cmath>

#define LEARN_RATE 0.7f
#define MOMENTUM 0.3f

using namespace std;

const float inputs[4][3] = {{0.0f, 0.0f, 1.0f},
							{0.0f, 1.0f, 1.0f},
							{1.0f, 0.0f, 1.0f},
							{1.0f, 1.0f, 1.0f}};
const float outputs[4] = {0.0f, 1.0f, 1.0f, 0.0f};

// Makes a random float from -10.0 to 10.0
float rand_float() {
	return -10.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 20.0f));
}

// Calculates the rms error
float rms(float ideal, float actual) {
	return ideal - actual;
}

class Neuron {
private:
	// Gets the node deltas
	float node_delta(float error, float output) {
		return -1 * rms(error, output) * output * (1.0f - output);
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
			for (int i = 0; i < this -> weights.size(); i++) {
				// Grab the previous weight
				float prev = this -> previous[i];
				// Set the new rate using the gradient
				float rate = LEARN_RATE * gradient(error, this -> sigmoid(layer_output)) + MOMENTUM * prev;
				this -> weights[i] -= rate;
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
		for (int i = 0; i < neurons.size(); i++) {
			ret.push_back(neurons[i] -> sigmoid(&inputs));
		}
		return ret;
	}
	//Does the whole backpropogation thing over a layer
	virtual void backpropogate(float error) {
		// This part does it for each neuron in this layer
		for (int i = 0; i < neurons.size(); i++) {
			if (!neurons.at(i) -> const_output) {
				vector<float> output = layer_above -> get_layer_output();
				neurons.at(i) -> backpropogate(error, &output);
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

// literally the only time interitance is useful
class InputLayer : public Layer {
public:
	vector<float> inputs;
	// Overrides get_layer_output and just returns the inputs
	vector<float> get_layer_output() {
		return inputs;
	}
	// This needs to be empty since you don't do it for backpropogation
	void backpropogate(float error) { }
	// Constructor for the inputlayer
	InputLayer(vector<float> outputs) {
		inputs = outputs;
	}
};

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
	vector<float> input_outputs = { 0.0f, 1.0f, 1.0f }; // such a poetic name
	InputLayer input_layer(input_outputs);

	// Create the middle layer and add neurons to it
	Layer middle_layer(input_layer);
	middle_layer.neurons = { &h1, &h2, &b2 };

	// Create the last layer
	Layer output_layer(middle_layer);
	output_layer.neurons = { &o1 };

	cout << "Layers created!" << endl;

	// Get the output of the layer
	vector<float> output = output_layer.get_layer_output();
	cout << "initial output is " << output[0] << endl;

	// Correct error
	float error = output_layer.correct_error(rms(1.0f, output_layer.get_layer_output()[0]));

	while (error > 0.05f) {
		error = output_layer.correct_error(error);
		cout << error << endl;
	}

	return 0;
}
