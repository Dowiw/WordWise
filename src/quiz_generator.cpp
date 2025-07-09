#include "quiz_generator.h"
#include <algorithm>
#include <stdexcept>
#ifdef _OPENMP
#include <omp.h>
#endif
using namespace std;

QuizGenerator::QuizGenerator(const WordBank& bank)
	: wordBank(bank), rng(random_device{}()) {}

vector<QuizQuestion> QuizGenerator::generateMultipleChoice(int numQuestions, int numChoices) {
	// variables: questions vector & words iterator
	vector<QuizQuestion> questions(numQuestions);
	const auto& words = wordBank.getWords();

	int wordsSize = words.size();

	// if number of words is less than choices throw error
	if (wordsSize < numChoices) throw runtime_error("Not enough words for choices");

	// shuffle questions (first take size, create indice, fill it with incrementing number, shuffle number)
	vector<int> indices(wordsSize);
	iota(indices.begin(), indices.end(), 0);
	shuffle(indices.begin(), indices.end(), rng);

	// get minimum number between size and numQuestions for the limit
	int loopBound = min(numQuestions, wordsSize);

	// parallelize question generation (say, if we are going to implemenent more than 5 questions)
	#pragma omp parallel for if(loopBound > 1)
	for (int i = 0; i < loopBound; ++i) {
		int correctIdx = indices[i]; // fill them up with the randomized indices

		QuizQuestion q; // declare new question
		q.word = words[correctIdx]; // assign each word to randomized number
		q.prompt = "What is the English meaning of '" + words[correctIdx].article + " " + words[correctIdx].word_de + "'?";

		// collect choices
		vector<string> options;
		options.push_back(words[correctIdx].word_en);

		// add random incorrect answers
		vector<int> distractors;
		for (int j = 0; j < wordsSize; ++j) {
			if (j != correctIdx) distractors.push_back(j);
		}

		// use a thread-local random engine for shuffling the distractors
		mt19937 local_rng(rng());
		shuffle(distractors.begin(), distractors.end(), local_rng);

		// get the english word based on the new shuffling and put it in options
		for (int k = 0; k < numChoices - 1 && k < (int)distractors.size(); ++k) {
			options.push_back(words[distractors[k]].word_en);
		}

		// shuffle the options once more along with the right answer
		shuffle(options.begin(), options.end(), local_rng);
		q.choices = options;

		// find correct index (make sure answer is there)
		bool found = false;
		for (int c = 0; c < (int)options.size(); ++c) {
			if (options[c] == words[correctIdx].word_en) { // very nice string compare
				q.correctIndex = c;
				found = true;
				break;
			}
		}
		if (!found)
			throw runtime_error("Bruh. There is no correct answer one of the words. Please contact Dowiw.");
		questions[i] = q; // assign question structure to questions vector
	}
	return questions;
}
