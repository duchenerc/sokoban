# CMakeList.txt : CMake project for sokoban, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.18)

# Add source to this project's executable.
add_executable (sokoban "sokoban.cpp" "Location.cpp" "Board.cpp")

# TODO: Add tests and install targets if needed.
