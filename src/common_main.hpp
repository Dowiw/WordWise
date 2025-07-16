#pragma once
#include "UI.hpp"
#include "Utils.hpp"
#include "WordDatabase.hpp"

#include <chrono>
#include <iostream>
#include <iomanip> // for table formatting

extern "C" { // done for compiling as C headers pls, compiler silence
#include <pthread.h>
#include <unistd.h>
}
using namespace std;
