cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "Macau")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

file(GLOB ALL_REQUIRED_DLL "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/bin/*.dll")
file(COPY ${ALL_REQUIRED_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

## If you want to link SFML statically
#set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system network REQUIRED)
add_executable(${PROJECT_NAME} main.cpp src/cards/Card.cpp src/cards/Card.h src/cards/PlayerCards.cpp src/cards/PlayerCards.h src/graphics/CardsImages.cpp src/graphics/CardsImages.h src/cards/Deck.cpp src/cards/Deck.h src/connection/Server.cpp src/connection/Server.h src/graphics/GUI.cpp src/graphics/GUI.h src/connection/ServerConnection.cpp src/connection/ServerConnection.h src/game/GameState.cpp src/game/GameState.h src/graphics/Button.cpp src/graphics/Button.h)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-network)