#pragma once
#include "word_entry.h"
#include <vector>

class NeuralNetwork {
public:
	NeuralNetwork();
	// predicts the user's likelihood of knowing a word (0.0 = doesn't know, 1.0 = knows well)
	float predict(const WordEntry& entry) const;

	// (Optional) train/update the model with user data
	void train(const std::vector<WordEntry>& data);
private:
	// placeholder for weights, layers, etc.
};
