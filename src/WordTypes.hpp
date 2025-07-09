#pragma once
#include <map>
#include <string>
#include <vector>
using namespace std;

// Word types in enumeration (int)
enum class WordType {
	NOUN,
	VERB,
	ADJECTIVE,
	PHRASE,
	OTHER
};

// word structure, contains: german & english word, WordType, difficulty, timesUsed & correct
struct Word {
	string german;
	string english;
	WordType type;
	int difficulty;
	int timesPracticed;
	int timesCorrect;
};

// basic user structure progress
struct UserProgress {
	int totalWordsLearned;
	int totalExercisesCompleted;
	map<WordType, int> typeProgress;
};
