#include "Utils.hpp" // map, string, vector, mutex, pthread
#include <iostream>
#include <unistd.h>
#include <omp.h>
#ifdef _WIN32 // ****** thing for windws clearing
#include <windows.h>
#endif
using namespace std;

volatile int processingFlag = 0;

void* loadingAnimationThreadFunc(void* arg) {
	int seconds = *(int*)arg;
	processingFlag = 1;
	for (int i = 0; i < seconds * 2 && processingFlag; ++i) {
		cout << "\rProcessing" << string(i % 4, '.') << flush;
	}
	if (processingFlag) {
		cout << "\rDone!          " << endl;
		processingFlag = 0;
	}
	return nullptr;
}

void runLoadingAnimation(int seconds) {
	static int arg;
	arg = seconds;
	loadingAnimationThreadFunc(&arg);
}

void stopLoadingAnimation() {
	processingFlag = 0;
}

void clearScreen() {
#ifdef _WIN32 // if compiled through windows
	system("cls");
#else
	system("clear"); // use unix command clear
#endif
}

void saveWordsInParallel(const vector<pair<string, bool>>& wordResults) {
	#pragma omp parallel for
	for (size_t i = 0; i < wordResults.size(); ++i) {
		wordDB.updateWordStats(wordResults[i].first, wordResults[i].second);
	}
	cout << "All word stats saved in parallel!\n";
}
