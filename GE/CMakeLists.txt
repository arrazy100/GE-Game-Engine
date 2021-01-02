cmake_minimum_required(VERSION 3.0.0)
project(GameEngineLibrary VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(GE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GE_HEADER ${GE_INCLUDE_DIR}/Box2D.h ${GE_INCLUDE_DIR}/Init.h ${GE_INCLUDE_DIR}/Physics.h ${GE_INCLUDE_DIR}/Shape.h ${GE_INCLUDE_DIR}/Sound.h ${GE_INCLUDE_DIR}/Sprite.h ${GE_INCLUDE_DIR}/Text.h ${GE_INCLUDE_DIR}/Tilemap.h)
set(GE_SOURCE ${GE_SOURCE_DIR}/Box2D.cpp ${GE_SOURCE_DIR}/Init.cpp ${GE_SOURCE_DIR}/Input.cpp ${GE_SOURCE_DIR}/Physics.cpp ${GE_SOURCE_DIR}/Shape.cpp ${GE_SOURCE_DIR}/Sound.cpp ${GE_SOURCE_DIR}/Sprite.cpp ${GE_SOURCE_DIR}/Text.cpp ${GE_SOURCE_DIR}/Tilemap.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/box2d)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tmx-tmx_1.1.0)

add_library(GameEngineLibrary ${GE_HEADER} ${GE_SOURCE})

target_include_directories(GameEngineLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(GameEngineLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/box2d/include)
target_include_directories(GameEngineLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tmx-tmx_1.1.0/src)

target_link_libraries(GameEngineLibrary PRIVATE SDL2::Main)
target_link_libraries(GameEngineLibrary PRIVATE SDL2::Image)
target_link_libraries(GameEngineLibrary PRIVATE SDL2::Mixer)
target_link_libraries(GameEngineLibrary PRIVATE SDL2::TTF)
target_link_libraries(GameEngineLibrary PRIVATE tmx)
target_link_libraries(GameEngineLibrary PRIVATE box2d)
