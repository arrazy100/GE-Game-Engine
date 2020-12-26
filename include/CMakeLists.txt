cmake_minimum_required(VERSION 3.0.0)
project(GameEngineLibrary VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
add_library(GameEngineLibrary Init.cpp Input.cpp Physics.cpp Shape.cpp Sprite.cpp Text.cpp)
target_link_libraries(GameEngineLibrary SDL2::Main)
target_link_libraries(GameEngineLibrary SDL2::Image)
target_link_libraries(GameEngineLibrary SDL2::TTF)