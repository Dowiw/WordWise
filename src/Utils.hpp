#pragma once
#include "WordDatabase.hpp" // map, string, vector, mutex

struct SaveWordArgs {
	string germanWord;
	bool wasCorrect;
};

void saveWordsInParallel(const vector<pair<string, bool>>& wordResults);

void* loadingAnimationThreadFunc(void* arg);
void* runLoadingAnimationThread(void* arg);
void stopLoadingAnimation();
void clearScreen();

extern volatile int processingFlag;

void loadingAnimation(int seconds);
void runLoadingAnimation(int seconds);
void stopLoadingAnimation();
void clearScreen();
