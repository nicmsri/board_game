cmake_minimum_required(VERSION 3.17)
project(board-game CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# configuration of SFML
#list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules)
find_package(SFML COMPONENTS graphics window system REQUIRED)

# main program
add_executable(board_game_main main.cpp setup.hpp game.hpp)
target_compile_options(board_game_main PRIVATE -Wall -Wextra)
target_link_options(board_game_main PRIVATE -fsanitize=address)
#target_include_directories(board_game_main PRIVATE ${SFML_INCLUDE_DIR})
#target_link_libraries(board_game_main PRIVATE ${SFML_LIBRARIES})
target_link_libraries(board_game_main PRIVATE sfml-graphics sfml-window sfml-system)

# testing
add_executable(testing testing.cpp setup.hpp game.hpp doctest.h)
target_compile_options(testing PRIVATE -Wall -Wextra)
target_link_options(testing PRIVATE -fsanitize=address)
target_link_libraries(testing PRIVATE sfml-graphics sfml-window sfml-system)
