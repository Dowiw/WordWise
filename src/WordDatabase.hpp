// WordDatabase.hpp
#pragma once
#include "WordTypes.hpp"
#include <string>
#include <vector>
#include <map>
#include <mutex>
using namespace std;

/*
	class structure for the main database:
	privates:
	words vector - container for default words
	userProgress - container for current user progress
	userAddedWords - container for words added by user
*/
class WordDatabase {
public:
	/*
	constructor:
	calls initializeDefaultWords()
	tries to load from user progress
	*/
	WordDatabase();

	/*
	destructor:
	calls saveProgress()
	*/
	~WordDatabase();

	// converts the string to its enum equivalent (used in printing)
	WordType stringToType(const string& typeStr);

	// converts the enum to its string equivalent (used in printing)
	string typeToString(WordType type);

	/*
	main function to load words into database vector words
	also loads initializes progress structure to zero
	***see private***
	*/
	void initializeDefaultWords();

	/*
	adds user word to separate userWords vector
	checks if word already exists based on german word passed
	*/
	void addUserWord(const Word& word);

	// getter for words that combines both the default ones & user words
	vector<Word> getAllWords() const;

	// getter for only the words in database
	vector<Word> getDefaultWords() const;

	// getter for only the words added by user
	vector<Word> getUserWords() const;

	/*
	pass in the germanWord string
	and bool if the answer to question was correct

	it will update both vectors
	*/
	void updateWordStats(const string& germanWord, bool wasCorrect);

	// when called, will increment the total exercises completed int
	void incrementExercisesCompleted();

	// increment value after words learned was saved in option 2
	void incrementWordsLearned(int count, WordType type);

	// getter for progress structure
	UserProgress getProgress() const;

	// function to save progress to "progress.dat" file
	void saveProgress() const;

	// function to load progress from "progress.dat" file
	void loadProgress();
private:
	vector<Word> words; // vector for default words
	UserProgress progress; // structure for user progress
	vector<Word> userAddedWords; // vector for added words
};

extern WordDatabase wordDB; // this is the db global variable
