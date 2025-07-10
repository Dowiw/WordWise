#include "Utils.hpp"
#include <iostream>
#include <unistd.h>
#include <chrono>
#ifdef _WIN32 // ****** thing for windws clearing
#include <windows.h>
#endif
using namespace std;

volatile int processingFlag = 0;
static pthread_t loadingThreadId;

// mutex for thread-safe console output in saveWordThreadFunc
extern pthread_mutex_t cout_mutex;

void* loadingAnimationThreadFunc(void* arg) {
	int seconds = *(int*)arg;
	processingFlag = 1;
	for (int i = 0; i < seconds * 2 && processingFlag; ++i) {
		cout << "\rProcessing" << string(i % 4, '.') << flush;
		usleep(500000); // 500 ms
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
	pthread_create(&loadingThreadId, nullptr, loadingAnimationThreadFunc, &arg);
	pthread_detach(loadingThreadId);
}

void stopLoadingAnimation() {
	processingFlag = 0;
	usleep(100000); // 100 ms
}

void clearScreen() {
#ifdef _WIN32 // if compiled through windows
	system("cls");
#else
	system("clear"); // use shell command clear
#endif
}

void* saveWordThreadFunc(void* arg) {
	SaveWordArgs* args = (SaveWordArgs*)arg;
	auto start = chrono::high_resolution_clock::now();

	pthread_mutex_lock(&cout_mutex);
	cout << "[Thread] Saving stats for word: '" << args->germanWord << "'...\n";
	pthread_mutex_unlock(&cout_mutex);

	wordDB.updateWordStats(args->germanWord, args->wasCorrect); // the main update method
	usleep(300000); // 0.3s for demo

	auto end = chrono::high_resolution_clock::now();
	double elapsed_ms = chrono::duration<double, milli>(end - start).count();

	pthread_mutex_lock(&cout_mutex);
	cout << "[Thread] Done saving: '" << args->germanWord << "' (" << elapsed_ms << " ms)\n";
	pthread_mutex_unlock(&cout_mutex);

	delete args;
	return nullptr;
}

void saveWordsInParallel(const vector<pair<string, bool>>& wordResults) {
	vector<pthread_t> threads(wordResults.size());

	for (size_t i = 0; i < wordResults.size(); ++i) {
		SaveWordArgs* args = new SaveWordArgs{wordResults[i].first, wordResults[i].second};
		pthread_create(&threads[i], nullptr, saveWordThreadFunc, args);
	}
	for (size_t i = 0; i < threads.size(); ++i) {
		pthread_join(threads[i], nullptr);
	}
	cout << "All word stats saved in parallel!\n";
}
