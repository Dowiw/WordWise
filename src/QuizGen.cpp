#include "QuizGen.hpp"
#include "WordDatabase.hpp"

#include <algorithm>
#include <random>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

// mutex for thread-safe console output
extern pthread_mutex_t cout_mutex;

// quiz data definitions (global)
FlashCardQuiz flashCardQuiz;
MultipleChoiceQuiz multipleChoiceQuiz;
TrueFalseQuiz trueFalseQuiz;

void* generateFlashCardQuiz(void* arg) {
	auto* allWords = static_cast<vector<Word>*>(arg); // cast allwords to point to a word vector structure

	flashCardQuiz.questions = *allWords; // set pointer to all words

	// shuffle all words
	shuffle(flashCardQuiz.questions.begin(), flashCardQuiz.questions.end(), default_random_engine(random_device{}()));

	// resize to 10 questions only
	if (flashCardQuiz.questions.size() > 10) flashCardQuiz.questions.resize(10);

	pthread_mutex_lock(&cout_mutex);
	cout << "[Thread] Flash Card quiz generated!\n";
	pthread_mutex_unlock(&cout_mutex);
	return (nullptr);
}

void* generateMultipleChoiceQuiz(void* arg) {
	auto* allWords = static_cast<vector<Word>*>(arg);

	multipleChoiceQuiz.questions.clear(); // clear previous if there is

	vector<Word> shuffled = *allWords;
	shuffle(shuffled.begin(), shuffled.end(), default_random_engine(random_device{}()));

	// get the min number if size is smaller than 10 etc..
	int total = min(10, (int)shuffled.size());
	for (int i = 0; i < total; ++i) {
		vector<string> options;
		options.push_back(shuffled[i].english); // add the correct english word
		while (options.size() < 4) {
			int idx = rand() % shuffled.size(); // randomly select other words
			if (shuffled[idx].english != shuffled[i].english &&
				find(options.begin(), options.end(), shuffled[idx].english) == options.end()) {
				options.push_back(shuffled[idx].english);
			}
		}
		shuffle(options.begin(), options.end(), default_random_engine(random_device{}())); // reshuffle
		multipleChoiceQuiz.questions.push_back({shuffled[i], options});
	}
	pthread_mutex_lock(&cout_mutex);
	cout << "[Thread] Multiple Choice quiz generated!\n";
	pthread_mutex_unlock(&cout_mutex);
	return nullptr;
}

void* generateTrueFalseQuiz(void* arg) {
	auto* allWords = static_cast<vector<Word>*>(arg);

	trueFalseQuiz.questions.clear();
	vector<Word> shuffled = *allWords;

	shuffle(shuffled.begin(), shuffled.end(), default_random_engine(random_device{}()));
	int total = min(10, (int)shuffled.size());

	for (int i = 0; i < total; ++i) {
		bool correct = rand() % 2;
		trueFalseQuiz.questions.push_back({shuffled[i], correct});
	}

	pthread_mutex_lock(&cout_mutex);
	cout << "[Thread] True/False quiz generated!\n";
	pthread_mutex_unlock(&cout_mutex);
	return nullptr;
}

void generateAllQuizzesParallel() {
	vector<Word> allWords = wordDB.getAllWords();
	pthread_t t1, t2, t3;
	cout << "[Main] Starting parallel quiz generation...\n";

	pthread_create(&t1, nullptr, generateFlashCardQuiz, &allWords);
	cout << "[Main] Flash Card quiz thread started.\n";
	pthread_create(&t2, nullptr, generateMultipleChoiceQuiz, &allWords);
	cout << "[Main] Multiple Choice quiz thread started.\n";
	pthread_create(&t3, nullptr, generateTrueFalseQuiz, &allWords);
	cout << "[Main] True/False quiz thread started.\n";

	pthread_join(t1, nullptr);
	cout << "[Main] Flash Card quiz thread finished.\n";
	pthread_join(t2, nullptr);
	cout << "[Main] Multiple Choice quiz thread finished.\n";
	pthread_join(t3, nullptr);
	cout << "[Main] True/False quiz thread finished.\n";

	cout << "All quizzes generated in parallel!\n";
}

void generateAllQuizzesSingleThreaded() {
	vector<Word> allWords = wordDB.getAllWords();
	cout << "\nStarting single threading for comparison... \n";
	generateFlashCardQuiz(&allWords);
	generateMultipleChoiceQuiz(&allWords);
	generateTrueFalseQuiz(&allWords);
	cout << "Single threading done!\n";
}
