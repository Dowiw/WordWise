#pragma once
#include <vector>
#include "WordTypes.hpp"

void displayWordList(const std::vector<Word>& words, int start = 0, int count = 10);
void learnSuggestedWords();
void manageMyWords();
void practiceExercises();
void reviewProgress();

// Quiz data structures and functions are now in QuizGen.hpp
#include "QuizGen.hpp"
