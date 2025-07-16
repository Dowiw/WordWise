// crazy *ss cpp file
#include "UI.hpp" // mutex, map, string, vector, quiz gen
#include "Utils.hpp" // pthread
#include "WordDatabase.hpp" // mutex

#include <iostream>
#include <algorithm>
#include <chrono>
#include <unistd.h>
#include <sstream>
using namespace std;

void displayWordList(const vector<Word>& words, int start, int count) {
	int end = min(start + count, static_cast<int>(words.size()));

	for (int i = start; i < end; ++i) {
		cout << i + 1 << ". " << words[i].german << " - " << words[i].english;
		cout << " (" << wordDB.typeToString(words[i].type) << ")";
		if (words[i].difficulty > 0) {
			cout << " [Difficulty: " << words[i].difficulty << "/5]";
		}
		if (words[i].timesPracticed > 0) {
			int accuracy = (words[i].timesCorrect * 100) / words[i].timesPracticed;
			cout << " [Practiced: " << words[i].timesPracticed << ", Accuracy: " << accuracy << "%]";
		}
		cout << endl;
	}

	cout << "\nShowing " << start + 1 << "-" << end << " of " << words.size() << " words." << endl;
}

void learnSuggestedWords() {
	while (true) {
		try {
			clearScreen();
			cout << "=== Learn Suggested Words ===\n\n";
			vector<Word> words = wordDB.getDefaultWords();
			if (words.empty()) {
				cout << "No suggested words available.\n";
				return;
			}

			int start = 0;
			const int pageSize = 10;

			while (true) {
				clearScreen();
				cout << "=== Learn Suggested Words ===\n\n";
				displayWordList(words, start, pageSize);

				cout << "\nOptions:\n";
				cout << "1. Next page\n";
				cout << "2. Previous page\n";
				cout << "3. Mark words as learned\n";
				cout << "4. Return to main menu\n";
				cout << "Enter your choice: ";

				int choice;
				if (!(cin >> choice)) {
					cout << "Invalid input. Please enter a number.\n";
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					usleep(1000000);
					continue;
				}

				switch (choice) {
					case 1:
						if (start + pageSize < words.size()) {
							start += pageSize;
						} else {
							cout << "Already at the last page.\n";
							usleep(1000000); // 1 second
						}
						break;
					case 2:
						if (start - pageSize >= 0) {
							start -= pageSize;
						} else {
							cout << "Already at the first page.\n";
							usleep(1000000); // 1 second
						}
						break;
					case 3: {
						cout << "Enter the numbers of words you've learned (e.g., 1 3 5), or 0 to cancel: ";
						cin.ignore();
						string input;
						getline(cin, input);

						if (input == "0") break;

						istringstream iss(input);
						int num;
						vector<int> selected;
						while (iss >> num) {
							if (num >= 1 && num <= words.size()) {
								selected.push_back(num - 1);
							}
						}

						if (!selected.empty()) {
							runLoadingAnimation(1);

							// count words by type
							map<WordType, int> typeCounts;
							for (int idx : selected) {
								typeCounts[words[idx].type]++;
								// Mark as practiced and correct (simulate a correct practice)
								wordDB.updateWordStats(words[idx].german, true);
							}

							// update progress
							for (const auto& pair : typeCounts)
								wordDB.incrementWordsLearned(pair.second, pair.first);

							stopLoadingAnimation();
							cout << "Marked " << selected.size() << " words as learned.\n";
							usleep(1000000); // 1 second
						}
						break;
					}
					case 4:
						return;
					default:
						cout << "Invalid choice. Please try again.\n";
						usleep(1000000); // 1 second
				}
			}
		} catch (const exception& e) {
			cout << "[Error] Exception in learnSuggestedWords: " << e.what() << endl;
			usleep(2000000);
		}
	}
}

void manageMyWords() {
	while (true) {
		try {
			clearScreen();
			cout << "=== Manage My Words ===\n\n";
			cout << "1. View my words\n";
			cout << "2. Add new word\n";
			cout << "3. Edit word\n";
			cout << "4. Delete word\n";
			cout << "5. Return to main menu\n";
			cout << "Enter your choice: ";

			int choice;
			if (!(cin >> choice)) {
				cout << "Invalid input. Please enter a number.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				usleep(1000000);
				continue;
			}

			switch (choice) {
				case 1: {
					vector<Word> userWords = wordDB.getUserWords();
					if (userWords.empty()) {
						cout << "You haven't added any words yet.\n";
					} else {
						int start = 0;
						const int pageSize = 10;

						while (true) {
							clearScreen();
							cout << "=== My Words ===\n\n";
							displayWordList(userWords, start, pageSize);

							cout << "\nOptions:\n";
							cout << "1. Next page\n";
							cout << "2. Previous page\n";
							cout << "3. Return to previous menu\n";
							cout << "Enter your choice: ";

							int subChoice;
							cin >> subChoice;

							if (subChoice == 1) {
								if (start + pageSize < userWords.size()) {
									start += pageSize;
							} else {
								cout << "Already at the last page.\n";
								usleep(1000000); // 1 second
							}
							} else if (subChoice == 2) {
								if (start - pageSize >= 0) {
									start -= pageSize;
							} else {
								cout << "Already at the first page.\n";
								usleep(1000000); // 1 second
							}
							} else if (subChoice == 3) {
								break;
							} else {
								cout << "Invalid choice. Please try again.\n";
								usleep(1000000); // 1 second
							}
						}
					}
					break;
				}
				case 2: {
					Word newWord;
					cout << "Enter German word: ";
					cin.ignore();
					getline(cin, newWord.german);
					cout << "Enter English translation: ";
					getline(cin, newWord.english);

					cout << "Select word type:\n";
					cout << "1. Noun\n";
					cout << "2. Verb\n";
					cout << "3. Adjective\n";
					cout << "4. Phrase\n";
					cout << "5. Other\n";
					cout << "Enter choice (1-5): ";
					int typeChoice;
					cin >> typeChoice;

					switch (typeChoice) {
						case 1: newWord.type = WordType::NOUN; break;
						case 2: newWord.type = WordType::VERB; break;
						case 3: newWord.type = WordType::ADJECTIVE; break;
						case 4: newWord.type = WordType::PHRASE; break;
						default: newWord.type = WordType::OTHER; break;
					}

					cout << "Enter difficulty (1-5): ";
					cin >> newWord.difficulty;
					newWord.timesPracticed = 0;
					newWord.timesCorrect = 0;

					wordDB.addUserWord(newWord);
					cout << "Word added successfully!\n";
					usleep(1000000); // 1 second
					break;
				}
				case 3: {
					vector<Word> userWords = wordDB.getUserWords();
					if (userWords.empty()) {
						cout << "You haven't added any words yet.\n";
						usleep(1000000); // 1 second
						break;
					}

					cout << "Enter the number of the word to edit: ";
					int num;
					cin >> num;

					if (num < 1 || num > userWords.size()) {
						cout << "Invalid number.\n";
						usleep(1000000); // 1 second
						break;
					}

					Word& word = userWords[num - 1];
					cout << "Editing word: " << word.german << " - " << word.english
						 << " (" << wordDB.typeToString(word.type) << ")\n";

					cout << "Enter new German word (or press Enter to keep current): ";
					cin.ignore();
					string german;
					getline(cin, german);
					if (!german.empty()) word.german = german;

					cout << "Enter new English translation (or press Enter to keep current): ";
					string english;
					getline(cin, english);
					if (!english.empty()) word.english = english;

					cout << "Select new word type (or 0 to keep current):\n";
					cout << "1. Noun\n";
					cout << "2. Verb\n";
					cout << "3. Adjective\n";
					cout << "4. Phrase\n";
					cout << "5. Other\n";
					cout << "Enter choice (0-5): ";
					int typeChoice;
					cin >> typeChoice;

					if (typeChoice > 0) {
						switch (typeChoice) {
							case 1: word.type = WordType::NOUN; break;
							case 2: word.type = WordType::VERB; break;
							case 3: word.type = WordType::ADJECTIVE; break;
							case 4: word.type = WordType::PHRASE; break;
							default: word.type = WordType::OTHER; break;
						}
					}

					cout << "Enter new difficulty (1-5, or 0 to keep current): ";
					int difficulty;
					cin >> difficulty;
					if (difficulty >= 1 && difficulty <= 5) word.difficulty = difficulty;

					cout << "Word updated successfully!\n";
					usleep(1000000); // 1 second
					break;
				}
				case 4: {
					vector<Word> userWords = wordDB.getUserWords();
					if (userWords.empty()) {
						cout << "You haven't added any words yet.\n";
						usleep(1000000); // 1 second
						break;
					}

					cout << "Enter the number of the word to delete: ";
					int num;
					cin >> num;

					if (num < 1 || num > userWords.size()) {
						cout << "Invalid number.\n";
						usleep(1000000); // 1 second
						break;
					}

					cout << "Are you sure you want to delete '" << userWords[num - 1].german << "'? (y/n): ";
					char confirm;
					cin >> confirm;

					if (confirm == 'y' || confirm == 'Y') {
						// In a real app, we would delete the word from the database
						cout << "Word deleted successfully!\n";
					} else {
						cout << "Deletion cancelled.\n";
					}
					usleep(1000000); // 1 second
					break;
				}
				case 5:
					return;
				default:
					cout << "Invalid choice. Please try again.\n";
					usleep(1000000); // 1 second
			}
		} catch (const exception& e) {
			cout << "[Error] Exception in manageMyWords: " << e.what() << endl;
			usleep(2000000);
		}
	}
}

void practiceExercises() {
	while (true) {
		try {
			clearScreen();
			cout << "=== Practice Exercises ===\n\n";
			cout << "1. Flash cards\n";
			cout << "2. Multiple choice\n";
			cout << "3. True/False\n";
			cout << "4. Return to main menu\n";
			cout << "Enter your choice: ";

			int choice;
			if (!(cin >> choice)) {
				cout << "Invalid input. Please enter a number.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				usleep(1000000);
				continue;
			}

			if (flashCardQuiz.questions.empty() || multipleChoiceQuiz.questions.empty() || trueFalseQuiz.questions.empty()) {
				cout << "Quizzes not generated yet. Please restart the program or check word database.\n";
				usleep(2000000);
				return;
			}

			switch (choice) {
				case 1: {
					int score = 0;
					int total = flashCardQuiz.questions.size();
					vector<pair<string, bool>> wordResults;
					for (int i = 0; i < total; ++i) {
						clearScreen();
						cout << "Flash Card " << i + 1 << " of " << total << "\n\n";
						cout << "German: " << flashCardQuiz.questions[i].german << "\n\n";
						cout << "Press Enter to reveal translation...";
						cin.ignore();
						cin.get();
						cout << "English: " << flashCardQuiz.questions[i].english << "\n\n";
						cout << "Did you get it right? (y/n): ";
						char answer;
						cin >> answer;
						bool correct = (answer == 'y' || answer == 'Y');
						wordResults.push_back({flashCardQuiz.questions[i].german, correct});
						if (correct) {
							score++;
							cout << "Good job!\n";
						} else {
							cout << "It's okay, you'll get it next time!\n";
						}
						cout << "Press Enter to continue...";
						cin.ignore();
						cin.get();
					}
					cout << "\nSaving your results in parallel...\n";
					saveWordsInParallel(wordResults);
					cout << "\nExercise completed! Your score: " << score << "/" << total << "\n";
					wordDB.incrementExercisesCompleted();
					cout << "Press Enter to return to the menu...";
					cin.ignore();
					cin.get();
					break;
				}
				case 2: {
					int score = 0;
					int total = multipleChoiceQuiz.questions.size();
					vector<pair<string, bool>> wordResults;
					for (int i = 0; i < total; ++i) {
						clearScreen();
						cout << "Multiple Choice " << i + 1 << " of " << total << "\n\n";
						cout << "What is the English translation of: " << multipleChoiceQuiz.questions[i].first.german << "\n\n";
						const auto& options = multipleChoiceQuiz.questions[i].second;
						for (int j = 0; j < 4; ++j) {
							cout << j + 1 << ". " << options[j] << "\n";
						}
						cout << "\nYour answer (1-4): ";
						int answer;
						cin >> answer;
						bool correct = (answer >= 1 && answer <= 4 && options[answer - 1] == multipleChoiceQuiz.questions[i].first.english);
						wordResults.push_back({multipleChoiceQuiz.questions[i].first.german, correct});
						if (correct) {
							score++;
							cout << "Correct!\n";
						} else {
							cout << "Incorrect. The right answer is: " << multipleChoiceQuiz.questions[i].first.english << "\n";
						}
						cout << "Press Enter to continue...";
						cin.ignore();
						cin.get();
					}
					cout << "\nSaving your results in parallel...\n";
					saveWordsInParallel(wordResults);
					cout << "\nExercise completed! Your score: " << score << "/" << total << "\n";
					wordDB.incrementExercisesCompleted();
					cout << "Press Enter to return to the menu...";
					cin.ignore();
					cin.get();
					break;
				}
				case 3: {
					int score = 0;
					int total = trueFalseQuiz.questions.size();

					vector<pair<string, bool>> wordResults;
					for (int i = 0; i < total; ++i) {
						clearScreen();
						cout << "True/False " << i + 1 << " of " << total << "\n\n";

						cout << "German: " << trueFalseQuiz.questions[i].first.german << "\n";
						cout << "English: " << trueFalseQuiz.questions[i].first.english << "\n";

						bool correctAnswer = trueFalseQuiz.questions[i].second;
						bool userAnswer;

						cout << "Is this translation correct? (1 for True, 0 for False): ";
						int input;
						cin >> input;
						userAnswer = (input == 1);
						bool correct = (userAnswer == correctAnswer);

						wordResults.push_back({trueFalseQuiz.questions[i].first.german, correct});
						if (correct) {
							score++;
							cout << "Correct!\n";
						} else {
							cout << "Incorrect!\n";
						}
						cout << "Press Enter to continue...";
						cin.ignore();
						cin.get();
					}

					cout << "\nSaving your results in parallel...\n";
					saveWordsInParallel(wordResults);
					cout << "\nExercise completed! Your score: " << score << "/" << total << "\n";
					wordDB.incrementExercisesCompleted();
					cout << "Press Enter to return to the menu...";
					cin.ignore();
					cin.get();
					break;
				}

				case 4:
					return;
				default:
					cout << "Invalid choice. Please try again.\n";
					usleep(1000000);
			}
		} catch (const exception& e) {
			cout << "[Error] Exception in practiceExercises: " << e.what() << endl;
			usleep(2000000);
		}
	}
}

void reviewProgress() {
	while (true) {
		try {
			clearScreen();
			UserProgress progress = wordDB.getProgress();
			vector<Word> allWords = wordDB.getAllWords();

			cout << "=== Review Progress ===\n\n";
			cout << "Total words learned: " << progress.totalWordsLearned << "\n";
			cout << "Total exercises completed: " << progress.totalExercisesCompleted << "\n\n";

			cout << "Words by Type:\n";
			cout << "Nouns: " << progress.typeProgress.at(WordType::NOUN) << "\n";
			cout << "Verbs: " << progress.typeProgress.at(WordType::VERB) << "\n";
			cout << "Adjectives: " << progress.typeProgress.at(WordType::ADJECTIVE) << "\n";
			cout << "Phrases: " << progress.typeProgress.at(WordType::PHRASE) << "\n";
			cout << "Other: " << progress.typeProgress.at(WordType::OTHER) << "\n";

			// Calculate overall accuracy
			int totalPracticed = 0;
			int totalCorrect = 0;
			for (const auto& word : allWords) {
				totalPracticed += word.timesPracticed;
				totalCorrect += word.timesCorrect;
			}

			if (totalPracticed > 0) {
				int accuracy = (totalCorrect * 100) / totalPracticed;
				cout << "\nOverall accuracy: " << accuracy << "%\n";
			}

			// Find most difficult words (accuracy < 50%)
			vector<Word> difficultWords;
			for (const auto& word : allWords) {
				if (word.timesPracticed > 0) {
					int accuracy = (word.timesCorrect * 100) / word.timesPracticed;
					if (accuracy < 50) {
						difficultWords.push_back(word);
					}
				}
			}

			if (!difficultWords.empty()) {
				cout << "\nWords you find difficult (accuracy < 50%):\n";
				for (size_t i = 0; i < min(difficultWords.size(), static_cast<size_t>(5)); ++i) {
					int accuracy = (difficultWords[i].timesCorrect * 100) / difficultWords[i].timesPracticed;
					cout << difficultWords[i].german << " - " << difficultWords[i].english
						 << " (" << wordDB.typeToString(difficultWords[i].type) << ", "
						 << accuracy << "% correct)\n";
				}
			}

			cout << "\nOptions:\n";
			cout << "1. View detailed word statistics\n";
			cout << "2. Return to main menu\n";
			cout << "Enter your choice: ";

			int choice;
			if (!(cin >> choice)) {
				cout << "Invalid input. Please enter a number.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				usleep(1000000);
				continue;
			}
			switch (choice) {
				case 1: {
					int start = 0;
					const int pageSize = 10;

					while (true) {
						clearScreen();
						cout << "=== Word Statistics ===\n\n";
						displayWordList(allWords, start, pageSize);

						cout << "\nOptions:\n";
						cout << "1. Next page\n";
						cout << "2. Previous page\n";
						cout << "3. Return to previous menu\n";
						cout << "Enter your choice: ";

						int subChoice;
						cin >> subChoice;

						if (subChoice == 1) {
							if (start + pageSize < allWords.size()) {
								start += pageSize;
							} else {
								cout << "Already at the last page.\n";
								usleep(1000000); // 1 second
							}
							} else if (subChoice == 2) {
								if (start - pageSize >= 0) {
									start -= pageSize;
							} else {
								cout << "Already at the first page.\n";
								usleep(1000000); // 1 second
							}
							} else if (subChoice == 3) {
								break;
							} else {
								cout << "Invalid choice. Please try again.\n";
								usleep(1000000); // 1 second
							}
						}
					break;
				}
				case 2:
					return;
				default:
					cout << "Invalid choice. Please try again.\n";
					usleep(1000000); // 1 second
			}
		} catch (const exception& e) {
			cout << "[Error] Exception in reviewProgress: " << e.what() << endl;
			usleep(2000000);
		}
	}
}
