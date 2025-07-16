#pragma once
#include "WordTypes.hpp" // map, string, vector

struct FlashCardQuiz {
	vector<Word> questions; // only questions for flash card as vector of word
};

struct MultipleChoiceQuiz {
	vector<pair<Word, vector<string>>> questions; // containter that pairs a word and a string
};

struct TrueFalseQuiz {
	vector<pair<Word, bool>> questions; // question only pairing each word with boolean as coorectness
};

// global quiz variables
extern FlashCardQuiz flashCardQuiz;
extern MultipleChoiceQuiz multipleChoiceQuiz;
extern TrueFalseQuiz trueFalseQuiz;

// function prototypes
void* generateFlashCardQuiz(void* arg);
void* generateMultipleChoiceQuiz(void* arg);
void* generateTrueFalseQuiz(void* arg);
void generateAllQuizzesParallel();

// function for testing
void generateAllQuizzesSingleThreaded();
