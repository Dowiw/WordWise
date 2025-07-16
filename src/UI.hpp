#pragma once
#include "WordTypes.hpp" // map, string, vector
#include "QuizGen.hpp" 

void displayWordList(const vector<Word>& words, int start = 0, int count = 10);
void learnSuggestedWords();
void manageMyWords();
void practiceExercises();
void reviewProgress();
