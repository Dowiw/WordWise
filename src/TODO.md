# WordWise – Smart Vocabulary Trainer: TODO

## Project Setup
- [x] Decide on data format (CSV)
- [ ] Set up project directory structure (`src/`, `data/`, `CMakeLists.txt`)
- [ ] Create initial `CMakeLists.txt` for build system

## Core Classes & Modules
- [ ] Implement `WordEntry` struct (represents a vocabulary word)
- [ ] Implement `WordBank` class
    - [ ] Load/save words from/to CSV
    - [ ] Add, update, remove words
    - [ ] Fallback to builtins if user bank is empty
- [ ] Implement `QuizGenerator` class
    - [ ] Generate multiple choice questions
    - [ ] Select words for quiz (initial: random, later: neural net)
    - [ ] Support fill-in-the-blank, other types (optional)
    - [ ] Use **OpenMP** for parallel scoring or quiz generation
- [ ] Implement `NeuralNetwork` class
    - [ ] Feedforward predict method
    - [ ] (Optional) Training logic for adaptive difficulty
    - [ ] Integrate with quiz selection (OpenMP for batch scoring, if needed)
- [ ] Implement `StatsTracker` class (optional)
    - [ ] Track right/wrong for each word
    - [ ] Update stats in user CSV after quiz
- [ ] Implement `CSVUtils` module or functions
    - [ ] Parse and serialize CSV files
    - [ ] Handle headers, missing fields, escaping

## CLI / Main Application
- [ ] Implement main menu (add word, review words, quiz, exit)
- [ ] Integrate all modules in `main.cpp`
- [ ] Display user stats and feedback

## Data Files
- [ ] Create `data/user_words.csv` (empty or starter)
- [ ] Create `data/builtins.csv` (with sample vocab)

## Parallelization (OpenMP)
- [ ] Add OpenMP support in `CMakeLists.txt` and source files
- [ ] Use `#pragma omp parallel for` for:
    - [ ] Scoring words with neural network
    - [ ] Generating quiz questions (if batch processing)
- [ ] Ensure thread-safety of any shared data structures

## Extensions (Optional, for later)
- [ ] GUI using Qt/SFML
- [ ] Support for multiple languages
- [ ] Export/import word banks
- [ ] Detailed quiz history/analytics
- [ ] Spaced repetition scheduling

---

**Tip:**  
Check off items as you go!  
Feel free to add new ideas and features below.

## Notes / Ideas

- Use OpenMP pragmas for simple, safe parallelism.
- Use `#pragma once` in each header file.
- Keep code modular: one class/module per file.
- Consider a CSV library if built-in parsing is not sufficient.

## Main Ideation

Project: WordWise – Smart Vocabulary Trainer

The goal of this project is to design and implement a parallelized 
language-learning assistant that helps users store, organize, and practice 
foreign vocabulary.

The system will apply parallel programming techniques to improve the performance of
- text analysis
- quiz generation
- real-time feedback

demonstrating the power of parallelism in enhancing user experience for real-world, interactive applications. 

Many students and young adults are actively learning new languages, but 
most apps focus on fixed word lists rather than allowing users to input, 
review, and memorize their own vocabulary.

Few platforms offer the flexibility to build a personalized word bank and revisit those words 
through customized exercises. This project addresses that gap by 
creating an app that lets users actively manage their vocabulary and 
review it through tailored quizzes. If a user hasn’t added any words yet, 
the app provides a built-in vocabulary section to ensure continuous 
learning. Parallel computing techniques are used to maintain high 
responsiveness and handle growing word sets efficiently.