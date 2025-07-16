#include "WordDatabase.hpp" // for class structure & word & wordtype, libs: mutex, map, string, vector

// standard c++ headers only for this CPP file
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

WordDatabase wordDB;
mutex mtx; // very important, makes threads wait when accessing function

WordDatabase::WordDatabase() {
	initializeDefaultWords();
	loadProgress();
}

WordDatabase::~WordDatabase() {
	saveProgress();
}

WordType WordDatabase::stringToType(const string& typeStr) {
	if (typeStr == "noun") return WordType::NOUN;
	if (typeStr == "verb") return WordType::VERB;
	if (typeStr == "adjective") return WordType::ADJECTIVE;
	if (typeStr == "phrase") return WordType::PHRASE;
	return WordType::OTHER;
}

string WordDatabase::typeToString(WordType type) {
	switch (type) {
		case WordType::NOUN: return "noun";
		case WordType::VERB: return "verb";
		case WordType::ADJECTIVE: return "adjective";
		case WordType::PHRASE: return "phrase";
		default: return "other";
	}
}

void WordDatabase::initializeDefaultWords() {
	// word structure as follows: german, english, wordtype, difficulty, timesPractice, timesCorrect
	words.clear();

	// maybe i should use csv loading instead..
	vector<Word> defaultWords = {
		// Basic phrases
		{"Hallo", "Hello", WordType::PHRASE, 1, 0, 0},
		{"Tschüss", "Goodbye", WordType::PHRASE, 1, 0, 0},
		{"Danke", "Thank you", WordType::PHRASE, 1, 0, 0},
		{"Bitte", "Please/You're welcome", WordType::PHRASE, 1, 0, 0},
		{"Ja", "Yes", WordType::PHRASE, 1, 0, 0},
		{"Nein", "No", WordType::PHRASE, 1, 0, 0},
		{"Entschuldigung", "Sorry", WordType::PHRASE, 2, 0, 0},
		// Pronouns (treated as other)
		{"ich", "I", WordType::OTHER, 1, 0, 0},
		{"du", "you (informal)", WordType::OTHER, 1, 0, 0},
		{"er", "he", WordType::OTHER, 1, 0, 0},
		{"sie", "she/they", WordType::OTHER, 1, 0, 0},
		{"es", "it", WordType::OTHER, 1, 0, 0},
		{"wir", "we", WordType::OTHER, 1, 0, 0},
		{"ihr", "you (plural)", WordType::OTHER, 1, 0, 0},
		{"Sie", "you (formal)", WordType::OTHER, 1, 0, 0},
		// Common verbs
		{"sein", "to be", WordType::VERB, 1, 0, 0},
		{"haben", "to have", WordType::VERB, 1, 0, 0},
		{"gehen", "to go", WordType::VERB, 1, 0, 0},
		{"kommen", "to come", WordType::VERB, 1, 0, 0},
		{"machen", "to do/make", WordType::VERB, 1, 0, 0},
		{"sagen", "to say", WordType::VERB, 1, 0, 0},
		{"geben", "to give", WordType::VERB, 1, 0, 0},
		{"nehmen", "to take", WordType::VERB, 1, 0, 0},
		{"wissen", "to know", WordType::VERB, 1, 0, 0},
		{"sehen", "to see", WordType::VERB, 1, 0, 0},
		{"hören", "to hear", WordType::VERB, 1, 0, 0},
		{"sprechen", "to speak", WordType::VERB, 2, 0, 0},
		{"verstehen", "to understand", WordType::VERB, 2, 0, 0},
		{"lernen", "to learn", WordType::VERB, 1, 0, 0},
		{"arbeiten", "to work", WordType::VERB, 1, 0, 0},
		{"spielen", "to play", WordType::VERB, 1, 0, 0},
		{"lesen", "to read", WordType::VERB, 1, 0, 0},
		{"schreiben", "to write", WordType::VERB, 1, 0, 0},
		{"wohnen", "to live/reside", WordType::VERB, 1, 0, 0},
		{"lieben", "to love", WordType::VERB, 1, 0, 0},
		{"mögen", "to like", WordType::VERB, 1, 0, 0},
		{"essen", "to eat", WordType::VERB, 1, 0, 0},
		{"trinken", "to drink", WordType::VERB, 1, 0, 0},
		{"schlafen", "to sleep", WordType::VERB, 1, 0, 0},
		{"kaufen", "to buy", WordType::VERB, 1, 0, 0},
		{"bezahlen", "to pay", WordType::VERB, 1, 0, 0},
		{"kosten", "to cost", WordType::VERB, 1, 0, 0},
		{"öffnen", "to open", WordType::VERB, 1, 0, 0},
		{"schließen", "to close", WordType::VERB, 1, 0, 0},
		{"beginnen", "to begin", WordType::VERB, 1, 0, 0},
		{"enden", "to end", WordType::VERB, 1, 0, 0},
		{"helfen", "to help", WordType::VERB, 1, 0, 0},
		{"suchen", "to search", WordType::VERB, 1, 0, 0},
		{"finden", "to find", WordType::VERB, 1, 0, 0},
		{"denken", "to think", WordType::VERB, 1, 0, 0},
		{"glauben", "to believe", WordType::VERB, 2, 0, 0},
		{"warten", "to wait", WordType::VERB, 1, 0, 0},
		{"brauchen", "to need", WordType::VERB, 1, 0, 0},
		{"versuchen", "to try", WordType::VERB, 1, 0, 0},
		{"vergessen", "to forget", WordType::VERB, 2, 0, 0},
		{"erinnern", "to remember", WordType::VERB, 2, 0, 0},
		{"treffen", "to meet", WordType::VERB, 1, 0, 0},
		{"besuchen", "to visit", WordType::VERB, 1, 0, 0},
		{"anrufen", "to call", WordType::VERB, 1, 0, 0},
		{"antworten", "to answer", WordType::VERB, 1, 0, 0},
		{"fragen", "to ask", WordType::VERB, 1, 0, 0},
		{"zeigen", "to show", WordType::VERB, 1, 0, 0},
		{"erklären", "to explain", WordType::VERB, 2, 0, 0},
		{"studieren", "to study", WordType::VERB, 1, 0, 0},
		// Nouns - people
		{"Mann", "man", WordType::NOUN, 1, 0, 0},
		{"Frau", "woman", WordType::NOUN, 1, 0, 0},
		{"Kind", "child", WordType::NOUN, 1, 0, 0},
		{"Freund", "friend (male)", WordType::NOUN, 1, 0, 0},
		{"Freundin", "friend (female)", WordType::NOUN, 1, 0, 0},
		{"Lehrer", "teacher (male)", WordType::NOUN, 1, 0, 0},
		{"Lehrerin", "teacher (female)", WordType::NOUN, 1, 0, 0},
		{"Schüler", "student (male)", WordType::NOUN, 1, 0, 0},
		{"Schülerin", "student (female)", WordType::NOUN, 1, 0, 0},
		{"Arzt", "doctor (male)", WordType::NOUN, 1, 0, 0},
		{"Ärztin", "doctor (female)", WordType::NOUN, 1, 0, 0},
		{"Chef", "boss (male)", WordType::NOUN, 1, 0, 0},
		{"Chefin", "boss (female)", WordType::NOUN, 1, 0, 0},
		{"Kollege", "colleague (male)", WordType::NOUN, 1, 0, 0},
		{"Kollegin", "colleague (female)", WordType::NOUN, 1, 0, 0},
		// Nouns - places
		{"Haus", "house", WordType::NOUN, 1, 0, 0},
		{"Wohnung", "apartment", WordType::NOUN, 1, 0, 0},
		{"Zimmer", "room", WordType::NOUN, 1, 0, 0},
		{"Küche", "kitchen", WordType::NOUN, 1, 0, 0},
		{"Bad", "bathroom", WordType::NOUN, 1, 0, 0},
		{"Schlafzimmer", "bedroom", WordType::NOUN, 1, 0, 0},
		{"Stadt", "city", WordType::NOUN, 1, 0, 0},
		{"Dorf", "village", WordType::NOUN, 1, 0, 0},
		{"Land", "country", WordType::NOUN, 1, 0, 0},
		{"Straße", "street", WordType::NOUN, 1, 0, 0},
		{"Platz", "square", WordType::NOUN, 1, 0, 0},
		{"Park", "park", WordType::NOUN, 1, 0, 0},
		{"Restaurant", "restaurant", WordType::NOUN, 1, 0, 0},
		{"Café", "cafe", WordType::NOUN, 1, 0, 0},
		{"Supermarkt", "supermarket", WordType::NOUN, 1, 0, 0},
		{"Geschäft", "shop", WordType::NOUN, 1, 0, 0},
		{"Bank", "bank", WordType::NOUN, 1, 0, 0},
		{"Post", "post office", WordType::NOUN, 1, 0, 0},
		{"Bahnhof", "train station", WordType::NOUN, 1, 0, 0},
		{"Flughafen", "airport", WordType::NOUN, 1, 0, 0},
		{"Krankenhaus", "hospital", WordType::NOUN, 1, 0, 0},
		{"Schule", "school", WordType::NOUN, 1, 0, 0},
		{"Universität", "university", WordType::NOUN, 1, 0, 0},
		{"Arbeit", "work", WordType::NOUN, 1, 0, 0},
		{"Büro", "office", WordType::NOUN, 1, 0, 0},
		// Nouns - food
		{"Essen", "food", WordType::NOUN, 1, 0, 0},
		{"Wasser", "water", WordType::NOUN, 1, 0, 0},
		{"Kaffee", "coffee", WordType::NOUN, 1, 0, 0},
		{"Tee", "tea", WordType::NOUN, 1, 0, 0},
		{"Milch", "milk", WordType::NOUN, 1, 0, 0},
		{"Brot", "bread", WordType::NOUN, 1, 0, 0},
		{"Butter", "butter", WordType::NOUN, 1, 0, 0},
		{"Käse", "cheese", WordType::NOUN, 1, 0, 0},
		{"Fleisch", "meat", WordType::NOUN, 1, 0, 0},
		{"Huhn", "chicken", WordType::NOUN, 1, 0, 0},
		{"Fisch", "fish", WordType::NOUN, 1, 0, 0},
		{"Ei", "egg", WordType::NOUN, 1, 0, 0},
		{"Obst", "fruit", WordType::NOUN, 1, 0, 0},
		{"Gemüse", "vegetables", WordType::NOUN, 1, 0, 0},
		{"Apfel", "apple", WordType::NOUN, 1, 0, 0},
		{"Banane", "banana", WordType::NOUN, 1, 0, 0},
		{"Orange", "orange", WordType::NOUN, 1, 0, 0},
		{"Tomate", "tomato", WordType::NOUN, 1, 0, 0},
		{"Kartoffel", "potato", WordType::NOUN, 1, 0, 0},
		{"Reis", "rice", WordType::NOUN, 1, 0, 0},
		{"Nudeln", "pasta", WordType::NOUN, 1, 0, 0},
		{"Salz", "salt", WordType::NOUN, 1, 0, 0},
		{"Pfeffer", "pepper", WordType::NOUN, 1, 0, 0},
		{"Zucker", "sugar", WordType::NOUN, 1, 0, 0},
		{"Eis", "ice cream", WordType::NOUN, 1, 0, 0},
		{"Kuchen", "cake", WordType::NOUN, 1, 0, 0},
		// Nouns - time
		{"Zeit", "time", WordType::NOUN, 1, 0, 0},
		{"Uhr", "clock/hour", WordType::NOUN, 1, 0, 0},
		{"Minute", "minute", WordType::NOUN, 1, 0, 0},
		{"Stunde", "hour", WordType::NOUN, 1, 0, 0},
		{"Tag", "day", WordType::NOUN, 1, 0, 0},
		{"Woche", "week", WordType::NOUN, 1, 0, 0},
		{"Monat", "month", WordType::NOUN, 1, 0, 0},
		{"Jahr", "year", WordType::NOUN, 1, 0, 0},
		{"Montag", "Monday", WordType::NOUN, 1, 0, 0},
		{"Dienstag", "Tuesday", WordType::NOUN, 1, 0, 0},
		{"Mittwoch", "Wednesday", WordType::NOUN, 1, 0, 0},
		{"Donnerstag", "Thursday", WordType::NOUN, 1, 0, 0},
		{"Freitag", "Friday", WordType::NOUN, 1, 0, 0},
		{"Samstag", "Saturday", WordType::NOUN, 1, 0, 0},
		{"Sonntag", "Sunday", WordType::NOUN, 1, 0, 0},
		{"Frühling", "spring", WordType::NOUN, 1, 0, 0},
		{"Sommer", "summer", WordType::NOUN, 1, 0, 0},
		{"Herbst", "autumn", WordType::NOUN, 1, 0, 0},
		{"Winter", "winter", WordType::NOUN, 1, 0, 0},
		{"Morgen", "morning", WordType::NOUN, 1, 0, 0},
		{"Mittag", "noon", WordType::NOUN, 1, 0, 0},
		{"Nachmittag", "afternoon", WordType::NOUN, 1, 0, 0},
		{"Abend", "evening", WordType::NOUN, 1, 0, 0},
		{"Nacht", "night", WordType::NOUN, 1, 0, 0},
		// Adjectives
		{"gut", "good", WordType::ADJECTIVE, 1, 0, 0},
		{"schlecht", "bad", WordType::ADJECTIVE, 1, 0, 0},
		{"groß", "big", WordType::ADJECTIVE, 1, 0, 0},
		{"klein", "small", WordType::ADJECTIVE, 1, 0, 0},
		{"alt", "old", WordType::ADJECTIVE, 1, 0, 0},
		{"neu", "new", WordType::ADJECTIVE, 1, 0, 0},
		{"jung", "young", WordType::ADJECTIVE, 1, 0, 0},
		{"lang", "long", WordType::ADJECTIVE, 1, 0, 0},
		{"kurz", "short", WordType::ADJECTIVE, 1, 0, 0},
		{"teuer", "expensive", WordType::ADJECTIVE, 1, 0, 0},
		{"billig", "cheap", WordType::ADJECTIVE, 1, 0, 0},
		{"leicht", "easy/light", WordType::ADJECTIVE, 1, 0, 0},
		{"schwer", "hard/heavy", WordType::ADJECTIVE, 1, 0, 0},
		{"schnell", "fast", WordType::ADJECTIVE, 1, 0, 0},
		{"langsam", "slow", WordType::ADJECTIVE, 1, 0, 0},
		{"schön", "beautiful", WordType::ADJECTIVE, 1, 0, 0},
		{"hässlich", "ugly", WordType::ADJECTIVE, 1, 0, 0},
		{"glücklich", "happy", WordType::ADJECTIVE, 1, 0, 0},
		{"traurig", "sad", WordType::ADJECTIVE, 1, 0, 0},
		{"interessant", "interesting", WordType::ADJECTIVE, 1, 0, 0},
		{"langweilig", "boring", WordType::ADJECTIVE, 1, 0, 0},
		{"wichtig", "important", WordType::ADJECTIVE, 1, 0, 0},
		{"unwichtig", "unimportant", WordType::ADJECTIVE, 1, 0, 0},
		{"richtig", "right/correct", WordType::ADJECTIVE, 1, 0, 0},
		{"falsch", "wrong", WordType::ADJECTIVE, 1, 0, 0},
		{"stark", "strong", WordType::ADJECTIVE, 1, 0, 0},
		{"schwach", "weak", WordType::ADJECTIVE, 1, 0, 0},
		{"heiß", "hot", WordType::ADJECTIVE, 1, 0, 0},
		{"kalt", "cold", WordType::ADJECTIVE, 1, 0, 0},
		{"warm", "warm", WordType::ADJECTIVE, 1, 0, 0},
		{"kühl", "cool", WordType::ADJECTIVE, 1, 0, 0},
		{"hell", "light/bright", WordType::ADJECTIVE, 1, 0, 0},
		{"dunkel", "dark", WordType::ADJECTIVE, 1, 0, 0},
		{"voll", "full", WordType::ADJECTIVE, 1, 0, 0},
		{"leer", "empty", WordType::ADJECTIVE, 1, 0, 0},
		{"sauber", "clean", WordType::ADJECTIVE, 1, 0, 0},
		{"schmutzig", "dirty", WordType::ADJECTIVE, 1, 0, 0},
		{"gesund", "healthy", WordType::ADJECTIVE, 1, 0, 0},
		{"krank", "sick", WordType::ADJECTIVE, 1, 0, 0},
		{"frei", "free", WordType::ADJECTIVE, 1, 0, 0},
		{"besetzt", "occupied", WordType::ADJECTIVE, 1, 0, 0},
		{"fertig", "ready/finished", WordType::ADJECTIVE, 1, 0, 0}
	};
	words = defaultWords;
	progress.totalWordsLearned = 0;
	progress.totalExercisesCompleted = 0;
	progress.typeProgress[WordType::NOUN] = 0;
	progress.typeProgress[WordType::VERB] = 0;
	progress.typeProgress[WordType::ADJECTIVE] = 0;
	progress.typeProgress[WordType::PHRASE] = 0;
	progress.typeProgress[WordType::OTHER] = 0;
}

void WordDatabase::addUserWord(const Word& word) {
	lock_guard<mutex> guard(mtx);

	// prevent duplicate user words by German spelling (lambda)
	auto it = find_if(userAddedWords.begin(), userAddedWords.end(), [&](const Word& w) {
		return w.german == word.german;
	});
	if (it == userAddedWords.end())
		userAddedWords.push_back(word);
	else
		cerr << "[Warning] Word '" << word.german << "' already exists in user words. Skipping.\n";
}

vector<Word> WordDatabase::getAllWords() const {
	lock_guard<mutex> guard(mtx);

	// create copy, do not pass reference
	vector<Word> allWords = words;
	allWords.insert(allWords.end(), userAddedWords.begin(), userAddedWords.end());
	return allWords;
}

vector<Word> WordDatabase::getDefaultWords() const {
	lock_guard<mutex> guard(mtx);
	return words;
}

vector<Word> WordDatabase::getUserWords() const {
	lock_guard<mutex> guard(mtx);
	return userAddedWords;
}

void WordDatabase::updateWordStats(const string& germanWord, bool wasCorrect) {
	lock_guard<mutex> guard(mtx);

	/*
	create update lambda:
	pass through address of the words in database
	change the times practiced & correct of the words
	*/
	auto update = [&](vector<Word>& wordList) {
		for (auto& word : wordList) {
			if (word.german == germanWord) {
				word.timesPracticed++;
				if (wasCorrect) word.timesCorrect++;
				return true;
			}
		}
		return false;
	};

	// call lambda & update accordingly, throw error if not found at all
	bool found = update(words);
	if (!found)
		found = update(userAddedWords);
	if (!found)
		cerr << "[Warning] Word '" << germanWord << "' is not found in any list. Contact Dowiw.\n";
}

void WordDatabase::incrementExercisesCompleted() {
	lock_guard<mutex> guard(mtx);
	progress.totalExercisesCompleted++;
}

void WordDatabase::incrementWordsLearned(int count, WordType type) {
	lock_guard<mutex> guard(mtx);
	progress.totalWordsLearned += count;
	progress.typeProgress[type] += count;
}

UserProgress WordDatabase::getProgress() const {
	lock_guard<mutex> guard(mtx);
	UserProgress dynamicProgress = progress; // update to current word progres

	// dynamically count words learned (timesCorrect > 0)
	int learnedCount = 0;
	map<WordType, int> typeCounts;

	// count each times a word & tupe is correct in the words vector
	for (const auto& word : words) {
		if (word.timesCorrect > 0) {
			learnedCount++;
			typeCounts[word.type]++;
		}
	}

	// do the same for user words
	for (const auto& word : userAddedWords) {
		if (word.timesCorrect > 0) {
			learnedCount++;
			typeCounts[word.type]++;
		}
	}

	// assign the value to the copy
	dynamicProgress.totalWordsLearned = learnedCount;

	// assign each pair of progress to the typeProgress map
	for (auto& pair : dynamicProgress.typeProgress) {
		pair.second = typeCounts[pair.first];
	}

	return dynamicProgress;
}

void WordDatabase::saveProgress() const {
	lock_guard<mutex> guard(mtx);
	try {
		ofstream file("progress.dat"); // similar to Open & Create by default in C, opens "progress.dat"

		// if file cannot be opened for any reason throw an error
		if (!file) throw ios_base::failure("Cannot open progress.dat for writing. Contact Dowiw or try resolving file permissions.");

		// write progress for total words & exercises first
		file << progress.totalWordsLearned << endl;
		file << progress.totalExercisesCompleted << endl;

		// then write progress for each word type (enum)
		for (const auto& pair : progress.typeProgress) {
			file << static_cast<int>(pair.first) << " " << pair.second << endl;
		}

		// then write data for each word using the best static cast
		file << "===DEFAULT WORDS===" << endl;
		for (const auto& word : words) {
			file << word.german << "|" << static_cast<int>(word.type) << "|"
				 << word.timesPracticed << "|" << word.timesCorrect << endl;
		}
	   file << "===USER WORDS===" << endl;
	   for (const auto& word : userAddedWords) {
		   file << word.german << "|" << word.english << "|" << static_cast<int>(word.type) << "|"
				<< word.timesPracticed << "|" << word.timesCorrect << endl;
	   }
		file.close(); // end file manipulation
	} catch (const exception& e) {
		cerr << "[Error] Failed to save progress: " << e.what() << endl;
	}
}

void WordDatabase::loadProgress() {
	lock_guard<mutex> guard(mtx);
	try {
		ifstream file("progress.dat");
		if (!file) { // no progress file yet (which means function should exit)
			cout << "No progress.dat file. Will make one during exit... ";
			return;
		}

		// take first two lines as data
		file >> progress.totalWordsLearned;
		file >> progress.totalExercisesCompleted;

		// take in each data from each number of the file stream
		int typeInt, count;
		for (int i = 0; i < 5; ++i) {
			file >> typeInt >> count;
			progress.typeProgress[static_cast<WordType>(typeInt)] = count;
		}

		// take each line, ignore if there is normal empty
		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;
			if (line == "===USER WORDS===") break; // first load default words

			// find positions for
			size_t pos1 = line.find('|');
			size_t pos2 = line.find('|', pos1 + 1);
			size_t pos3 = line.find('|', pos2 + 1);

			// if npos is returned (meaning error, just skip the word)
			if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) continue;

			string german = line.substr(0, pos1); // get german string between line and 0

			// get word type between second pos and pos 1
			WordType type = static_cast<WordType>(stoi(line.substr(pos1 + 1, pos2 - pos1 - 1)));

			// get int between pos 2 and pos 3
			int practiced = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
			int correct = stoi(line.substr(pos3 + 1)); // last value is correct
			for (auto& word : words) {
				if (word.german == german) {
					word.type = type;
					word.timesPracticed = practiced;
					word.timesCorrect = correct;
					break;
				}
			}
			// default words to big to fail
		}

		// now load user words, same logic, but also load English translation
		while (getline(file, line)) {
			if (line.empty()) continue;
			size_t pos1 = line.find('|');
			size_t pos2 = line.find('|', pos1 + 1);
			size_t pos3 = line.find('|', pos2 + 1);
			size_t pos4 = line.find('|', pos3 + 1);
			if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos || pos4 == string::npos) continue;
				string german = line.substr(0, pos1);
			string english = line.substr(pos1 + 1, pos2 - pos1 - 1);
			WordType type = static_cast<WordType>(stoi(line.substr(pos2 + 1, pos3 - pos2 - 1)));
			int practiced = stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));
			int correct = stoi(line.substr(pos4 + 1));
			bool found = false;
			for (auto& word : userAddedWords) {
				if (word.german == german) {
					cerr << "[Info] Duplicate user word found during load: '" << german << "'. Updating existing entry." << endl;
					word.english = english;
					word.type = type;
					word.timesPracticed = practiced;
					word.timesCorrect = correct;
					found = true;
					break;
				}
			}
			if (!found) {
				userAddedWords.push_back({german, english, type, 1, practiced, correct});
			}
		}
		file.close(); // close file manipulation
	} catch (const exception& e) {
		cerr << "[Error] Failed to load progress: " << e.what() << endl;
	}
}
