#pragma once
#include <string>
using namespace std;

struct WordEntry {
	string article; // german article
	string word_de; // german word
	string word_en; // english word
	int correctCount = 0; // number of times correct answer
	int incorrectCount = 0; // number of times wrong answer

	// add more fields as needed (e.g., part of speech, example sentence)
};
