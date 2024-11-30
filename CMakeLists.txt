cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

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

add_library(Clib 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libCforExternC/Clib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libCforExternC/Clib.h
)
add_executable(externCexperiment ${CMAKE_CURRENT_SOURCE_DIR}/src/externCexperiment.cpp)
target_link_libraries(externCexperiment PRIVATE Clib)

add_executable(inner_scope ${CMAKE_CURRENT_SOURCE_DIR}/src/inner_scope.cpp)

add_executable(threadsOrder ${CMAKE_CURRENT_SOURCE_DIR}/src/threadsOrder.cpp)

add_executable(variadic_arguments ${CMAKE_CURRENT_SOURCE_DIR}/src/variadic_arguments.cpp)

add_executable(platform_calculator ${CMAKE_CURRENT_SOURCE_DIR}/src/platform_calculator.cpp)
