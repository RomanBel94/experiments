cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(REMOVE_COMMAND rm -rf)


project(Experiments 
    VERSION 1.0.0
    DESCRIPTION "Repository for excercises and experiments"
    LANGUAGES C CXX
)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_compile_options(/utf-8)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    add_compile_options(-Wall)
endif()

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

add_executable(countWords ${CMAKE_CURRENT_SOURCE_DIR}/src/countWords.cpp)

add_executable(percentDiff ${CMAKE_CURRENT_SOURCE_DIR}/src/percentDiff.cpp)

add_executable(numberRenderer ${CMAKE_CURRENT_SOURCE_DIR}/src/numberRenderer.cpp)

add_executable(lambdaInheritance ${CMAKE_CURRENT_SOURCE_DIR}/src/lambdaInheritance.cpp)
set_property(TARGET lambdaInheritance PROPERTY CXX_STANDARD 17)

add_executable(placementNew ${CMAKE_CURRENT_SOURCE_DIR}/src/placementNew.cpp)

add_executable(regex ${CMAKE_CURRENT_SOURCE_DIR}/src/regex.cpp)

add_executable(trafic_light ${CMAKE_CURRENT_SOURCE_DIR}/src/trafic_light.cpp)
add_custom_command(TARGET trafic_light
    POST_BUILD
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/src/trafic_light_config.txt ${CMAKE_CURRENT_BINARY_DIR})

add_executable(ip_address_parser ${CMAKE_CURRENT_SOURCE_DIR}/src/ip_address_parser.cpp)

add_executable(Command_table_parser ${CMAKE_CURRENT_SOURCE_DIR}/src/Command_table_parser.cpp)
add_custom_command(TARGET Command_table_parser
    POST_BUILD
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/src/Command_table ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(hashgen ${CMAKE_CURRENT_SOURCE_DIR}/src/hashgen.cpp)

add_executable(er_kmac_generator ${CMAKE_CURRENT_SOURCE_DIR}/src/er_kmac_generator.cpp)

add_custom_target(
    full_clean
    COMMAND ${REMOVE_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/*
)

add_executable(optional ${CMAKE_CURRENT_SOURCE_DIR}/src/optional.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/optional.hpp)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_compile_options(optional PRIVATE /analyze)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    target_compile_options(optional PRIVATE -fanalyzer)
endif()

add_executable(blink_timer ${CMAKE_CURRENT_SOURCE_DIR}/src/blink_timer.cpp)

add_executable(morze_coder ${CMAKE_CURRENT_SOURCE_DIR}/src/morze_coder.cpp)

add_executable(accumulate_strings ${CMAKE_CURRENT_SOURCE_DIR}/src/accumulate_strings.cpp)

add_executable(dir_scanner ${CMAKE_CURRENT_SOURCE_DIR}/src/dir_scanner.cpp)
