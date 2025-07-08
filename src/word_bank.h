#pragma once
#include <vector>
#include <string>
#include "word_entry.h"
using namespace std;

class WordBank {
public:
	// load words from csv
	bool loadFromCSV(const string& filename);
	// save words to csv
	bool saveToCSV(const string& filename) const;

	void addWord(const WordEntry& entry);
	// remove a word by German word (word_de)
	bool removeWord(const string& word_de);

	// update a word by German word (word_de)
	bool updateWord(const string& word_de, const WordEntry& updatedEntry);

	// get all words
	const vector<WordEntry>& getWords() const;

	// check if empty
	bool isEmpty() const;

private:
	vector<WordEntry> words;
};
