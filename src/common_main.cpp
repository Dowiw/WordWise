#include "UI.hpp"
#include "Utils.hpp"
#include "WordDatabase.hpp"

#include <iostream>
extern "C" { // ensure none of the function are modified for the love of .. by linking as C
#include <pthread.h>
#include <unistd.h>
}
using namespace std;

// Global mutex for thread-safe console output
pthread_mutex_t cout_mutex = PTHREAD_MUTEX_INITIALIZER;

// Main function
int main() {
	try {
		// seed random number generator (how? just ask)
		srand(static_cast<unsigned int>(time(nullptr)));

		// Generate all quizzes in parallel at startup
		generateAllQuizzesParallel();

		// Wait for user to press Enter before starting the main menu
		cout << "\n[Main] Press Enter to start the German Language Learning Program..." << endl;
		cin.ignore();
		cin.get();

		while (true) {
			clearScreen();
			cout << "=== German Language Learning Program ===\n\n";
			cout << "Main Menu:\n";
			cout << "1. Learn Suggested Words\n";
			cout << "2. Manage My Words\n";
			cout << "3. Practice Exercises\n";
			cout << "4. Review Progress\n";
			cout << "5. Exit Program\n";
			cout << "Enter your choice: ";

			int choice;
			if (!(cin >> choice)) {
				cout << "Invalid input. Please enter a number.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				usleep(1000000);
				continue;
			}

			try {
				switch (choice) {
					case 1:
						learnSuggestedWords();
						break;
					case 2:
						manageMyWords();
						break;
					case 3:
						practiceExercises();
						break;
					case 4:
						reviewProgress();
						break;
					case 5:
						cout << "Saving progress and exiting program. Auf Wiedersehen!\n";
						return 0;
					default:
						cout << "Invalid choice. Please try again.\n";
						usleep(1000000); // 1 second
				}
			} catch (const std::exception& e) {
				cout << "[Error] Exception in menu action: " << e.what() << endl;
				usleep(2000000);
			}
		}
	} catch (const std::exception& e) {
		cout << "[Fatal Error] Unhandled exception: " << e.what() << endl;
		return 1;
	}
	return 0;
}
