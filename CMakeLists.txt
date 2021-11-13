# Copyright (c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

cmake_minimum_required(VERSION 3.17)



# Project

project(SPlanner)

message("Main directory: ${CMAKE_SOURCE_DIR}")



# Outputs

### Setup output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)		# .exe
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libs)	# .lib / .a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libs)	# .dll / .so



# Inputs

file(GLOB_RECURSE SOURCES "Include/SP/*" "Source/SP/*")



# Library

## Add Static library
add_library(SPlanner STATIC ${SOURCES})


## Include directories

target_include_directories(SPlanner PUBLIC Include)
target_include_directories(SPlanner PRIVATE Include/SP)


## Compile features

### Standard
target_compile_features(SPlanner PUBLIC c_std_11)
target_compile_features(SPlanner PUBLIC cxx_std_20)



# Options

## Add SPlanner's tests to build tree.
option(SPLANNER_BUILD_TESTS "Should build SA-Engine tests" OFF)


## Define continuous integration (CI) preprocessor.
if(SA_CI)
	target_compile_definitions(SPlanner PUBLIC SA_CI)
endif()



# Tests

### Enable testing for this directory and below.
enable_testing()



# Entrypoints

if(SPLANNER_BUILD_TESTS)
	add_subdirectory(Tests)
endif()
