#include "common_main.hpp"

// global mutex for thread-safe console output
pthread_mutex_t cout_mutex = PTHREAD_MUTEX_INITIALIZER;

// global variables to store timing results
static double parallelQuizGenMs = 0.0;
static double singleQuizGenMs = 0.0;

// forward declaration for single-threaded quiz generation
void generateAllQuizzesSingleThreaded();

// main function
int main() {
	try {
		// seed random number generator (how? just ask)
		srand(static_cast<unsigned int>(time(nullptr)));

		// performance analysis: Quiz Generation
		auto start_parallel = chrono::high_resolution_clock::now();
		generateAllQuizzesParallel();
		auto end_parallel = chrono::high_resolution_clock::now();
		parallelQuizGenMs = chrono::duration<double, milli>(end_parallel - start_parallel).count();

		auto start_single = chrono::high_resolution_clock::now();
		generateAllQuizzesSingleThreaded();
		auto end_single = chrono::high_resolution_clock::now();
		singleQuizGenMs = chrono::duration<double, milli>(end_single - start_single).count();

		// Wait for user to press Enter before starting the main menu
		cout << "\n[Main] Press Enter to start the German Language Learning Program..." << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore value
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
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

					// print performance analysis summary table before exit
					cout << "\n+-----------------------+-----------------------+-------------------+----------+\n";
					cout << "|      Operation       | Single-threaded (ms)  | Parallel (ms)     | Speedup  |\n";
					cout << "+-----------------------+-----------------------+-------------------+----------+\n";
					cout << "| Quiz Generation      | "
						 << setw(21) << left << singleQuizGenMs << "| "
						 << setw(16) << left << parallelQuizGenMs << "| ";
					if (parallelQuizGenMs > 0.0) {
						double speedup = singleQuizGenMs / parallelQuizGenMs;
						// set the output to only have the width to have 8 values max and .00 floating values
						cout << setw(8) << left << fixed << setprecision(2) << speedup << "x";
					} else {
						cout << setw(8) << left << "N/A";
					}
					cout << "   |\n";
					cout << "+-----------------------+-----------------------+-------------------+----------+\n";
					return 0;
					default:
						cout << "Invalid choice. Please try again.\n";
						usleep(1000000); // 1 second
				}
			} catch (const exception& e) {
				cout << "[Error] Exception in menu action: " << e.what() << endl;
				usleep(2000000);
			}
		}
	} catch (const exception& e) {
		cout << "[Fatal Error] Unhandled exception: " << e.what() << endl;
		return 1;
	}
	return 0;
}
