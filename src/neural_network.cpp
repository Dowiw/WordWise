#include "neural_network.h"

NeuralNetwork::NeuralNetwork() {
	// initialize weights, layers, etc. (placeholder)
}

float NeuralNetwork::predict(const WordEntry& entry) const {
	// simple heuristic: more correct answers = higher score, more incorrect = lower
	int total = entry.correctCount + entry.incorrectCount;
	if (total == 0) return 0.0f; // Not seen yet
	return static_cast<float>(entry.correctCount) / total;
}

void NeuralNetwork::train(const std::vector<WordEntry>& data) {
	// placeholder: in a real NN, update weights based on data
	// literally might not make the cut but anyway
}
