#pragma once
#include <pthread.h>
#include <vector>
#include <string>
#include "WordDatabase.hpp"

struct SaveWordArgs {
	std::string germanWord;
	bool wasCorrect;
};

void* saveWordThreadFunc(void* arg);
void saveWordsInParallel(const std::vector<std::pair<std::string, bool>>& wordResults);

void* loadingAnimationThreadFunc(void* arg);
void* runLoadingAnimationThread(void* arg);
void stopLoadingAnimation();
void clearScreen();

extern volatile int processingFlag;

void loadingAnimation(int seconds);
void runLoadingAnimation(int seconds);
void stopLoadingAnimation();
void clearScreen();
