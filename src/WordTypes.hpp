#pragma once
#include <map>
#include <string>
#include <vector>
using namespace std;

// word types in enumeration (int): Noun, Verb, Adjective, Phrase, Other
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

// basic user structure progress: totalLearned, totalExercise, progressForType of quiz
struct UserProgress {
	int totalWordsLearned;
	int totalExercisesCompleted;
	map<WordType, int> typeProgress;
};
