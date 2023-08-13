cmake_minimum_required(VERSION 3.24)
project(Project3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project3 src/main.cpp src/Tile.cpp src/Tile.h src/Board.cpp src/Board.h src/TextureManager.cpp src/TextureManager.h src/random.h src/random.cpp src/Time.h src/Leaderboard.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(c:/SFML/include/SFML)
target_link_libraries(Project3 sfml-system sfml-window sfml-graphics sfml-audio)