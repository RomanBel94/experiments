cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)

project(Experiments 
    VERSION 1.0.0
    DESCRIPTION "Repository for excercises and experiments"
    LANGUAGES C CXX
)

add_executable(myStrlen ${CMAKE_CURRENT_SOURCE_DIR}/src/myStrlen.c)
add_executable(findUniqueWithXOR ${CMAKE_CURRENT_SOURCE_DIR}/src/findUniqueWithXOR.cpp)
add_executable(nullptrNaming ${CMAKE_CURRENT_SOURCE_DIR}/src/nullptrNaming.cpp)
add_executable(recursivelyCallingMain ${CMAKE_CURRENT_SOURCE_DIR}/src/recursivelyCallingMain.cpp)
add_executable(templateFactorial ${CMAKE_CURRENT_SOURCE_DIR}/src/templateFactorial.cpp)
add_executable(twoSum ${CMAKE_CURRENT_SOURCE_DIR}/src/twoSum.cpp)
