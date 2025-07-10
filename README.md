# WordWise

A modern, parallelized German vocabulary learning app for the terminal.


## Features
- Flash card, multiple choice, and true/false quizzes (all generated in parallel for speed)
- Track your progress and word stats
- Add, edit, and manage your own words
- Review your most difficult words
- Animated loading and parallel saving of results
- Cross-platform (Linux, macOS, Windows with MinGW or WSL)

## Build Instructions

### Prerequisites
- C++17 compiler (e.g., g++, clang++)
- CMake 3.10 or newer
- POSIX threads (pthread)

### Build (Recommended)
```sh
cd /path/to/WordWise
mkdir build
cd build
cmake ..
make
```
The executable will be named `WordWise`.

### Build (Manual)
```sh
g++ src/common_main.cpp src/QuizGen.cpp src/UI.cpp src/Utils.cpp src/WordDatabase.cpp -o WordWise -lpthread
```

## Usage
Run the program:
```sh
./WordWise
```

Follow the on-screen menu to:
- Learn suggested words
- Manage your own word list
- Practice with different quiz types
- Review your progress and stats

## Data
- Your progress is saved in `progress.dat`.
- User-added words are tracked and can be edited or deleted.

## Notes
- Console output is thread-safe, so you won't see garbled messages even when saving or generating quizzes in parallel.
- If you run into any issues, please open an issue or suggest improvements!
- [ ] Might implement sentence generation using a neural network (doubt)
- [ ] Support for multiple languages
- [ ] Scale to more words for exercises
- [ ] Add option to specify word difficulty

---

> - Made with C++, KB, Fatih, Ferds, Ezra, Shukrulloh
> - Happy learning! 🇩🇪
