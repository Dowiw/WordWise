cmake_minimum_required(VERSION 3.10)
project(WordWise)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
	src/common_main.cpp
	src/QuizGen.cpp
	src/UI.cpp
	src/Utils.cpp
	src/WordDatabase.cpp
)

# Include directories
include_directories(src)

add_executable(WordWise ${SOURCES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
	target_link_libraries(WordWise PUBLIC OpenMP::OpenMP_CXX pthread)
else()
	target_link_libraries(WordWise PUBLIC pthread)
endif()
