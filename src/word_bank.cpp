#include "word_bank.h"
/*
	Make functions for word_bank.h
*/

#include <fstream> // file stream
#include <sstream> // string manipulation in file

// helper function to trim whitespace for a string
static string trim(const string& s) {
	// find first & last position of not these
	size_t start = s.find_first_not_of(" \t\n\r");
	size_t end = s.find_last_not_of(" \t\n\r");

	// if string exists do trimming, if not '\0'
	return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

bool WordBank::loadFromCSV(const string& filename) {
	words.clear();

	ifstream file(filename); // input file (basically retrive the file like open())
	if (!file.is_open()) return false; // do not do anything if file is open

	string line;
	getline(file, line); // skip header

	while (getline(file, line)) {
		// use ss for getline parsing
		stringstream ss(line); // replace the current string
		string article, word_de, word_en, correctStr, incorrectStr;
		getline(ss, article, ',');
		getline(ss, word_de, ',');
		getline(ss, word_en, ',');
		getline(ss, correctStr, ',');
		getline(ss, incorrectStr, ',');

		// parse wordentry
		WordEntry entry;
		entry.article = trim(article);
		entry.word_de = trim(word_de);
		entry.word_en = trim(word_en);
		entry.correctCount = correctStr.empty() ? 0 : stoi(correctStr);
		entry.incorrectCount = incorrectStr.empty() ? 0 : stoi(incorrectStr);
		words.push_back(entry);
	}
	return true;
}

bool WordBank::saveToCSV(const string& filename) const {
	ofstream file(filename); // output file (basically write into file as open('w'))
	if (!file.is_open()) return false; // if output file open, stop

	// write into file
	file << "article,word_de,word_en,correctCount,incorrectCount\n"; // header
	for (const auto& entry : words) {
		file << entry.article << ',' << entry.word_de << ',' << entry.word_en << ','
			 << entry.correctCount << ',' << entry.incorrectCount << '\n';
	}
	return true;
}

void WordBank::addWord(const WordEntry& entry) {
	words.push_back(entry);
}

bool WordBank::removeWord(const string& word_de) {
	for (auto it = words.begin(); it != words.end(); ++it) {
		if (it->word_de == word_de) {
			words.erase(it);
			return true;
		}
	}
	return false;
}

bool WordBank::updateWord(const string& word_de, const WordEntry& updatedEntry) {
	for (auto& entry : words) {
		if (entry.word_de == word_de) {
			entry = updatedEntry;
			return true;
		}
	}
	return false;
}

const vector<WordEntry>& WordBank::getWords() const {
	return words;
}

bool WordBank::isEmpty() const {
	return words.empty();
}
