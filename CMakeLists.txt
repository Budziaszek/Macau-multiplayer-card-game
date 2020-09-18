#cmake_minimum_required(VERSION 3.17)
#set(PROJECT_NAME "Macau")
#project(${PROJECT_NAME})
#add_executable(${PROJECT_NAME} main.cpp)
#
##set(SFML_STATIC_LIBRARIES TRUE)
#set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")
#
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#find_package(SFML 2.5.1 COMPONENTS audio, network, graphics, window, system REQUIRED)
#target_link_libraries(${PROJECT_NAME} sfml-audio, sfml-network, sfml-graphics, sfml-window, sfml-system)

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
#set(SFML_DIR "D:/Libraries/SFML/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)