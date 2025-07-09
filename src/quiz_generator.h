#pragma once
#include "word_bank.h"
#include <vector>
#include <string>
#include <random>
using namespace std;

struct QuizQuestion {
	string prompt; // the question shown to the user
	vector<string> choices; // multiple choice options
	int correctIndex; // index of the correct answer in choices
	WordEntry word; // the word this question is about
};

class QuizGenerator {
public:
	QuizGenerator(const WordBank& bank);
	// generate a set of random multiple choice questions (adaptive)
	vector<QuizQuestion> generateMultipleChoice(int numQuestions = 5, int numChoices = 4);
private:
	const WordBank& wordBank;
	mutable mt19937 rng; // what the hell is this?
	/*
	this value will change every time the class uses the value
	'mt19937' is a standard random engine, mutable allows this value to modify everytime it is called
	*/
};
