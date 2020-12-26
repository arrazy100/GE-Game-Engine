cmake_minimum_required(VERSION 3.0.0)
project(GameEngineExample VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(GameEngineExample main.cpp)
target_link_libraries(GameEngineExample GameEngineLibrary)
set_target_properties(GameEngineExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
