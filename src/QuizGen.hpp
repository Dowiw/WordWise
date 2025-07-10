#pragma once
#include "WordTypes.hpp"

#include <vector>
#include <string>
using namespace std;

struct FlashCardQuiz {
	vector<Word> questions;
};

struct MultipleChoiceQuiz {
	vector<pair<Word, vector<string>>> questions; // containter that pairs a word and a string
};

struct TrueFalseQuiz {
	vector<pair<Word, bool>> questions;
};

extern FlashCardQuiz flashCardQuiz;
extern MultipleChoiceQuiz multipleChoiceQuiz;
extern TrueFalseQuiz trueFalseQuiz;

void* generateFlashCardQuiz(void* arg);
void* generateMultipleChoiceQuiz(void* arg);
void* generateTrueFalseQuiz(void* arg);
void generateAllQuizzesParallel();
