cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)
################### Variables. ####################
# Change if you want modify path or other values. #
###################################################

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME ROV)
# Output Variables
set(OUTPUT_DEBUG Debug/bin)
set(OUTPUT_RELEASE Release/bin)

# Folders files
set(CPP_DIR_1 src)
set(CPP_DIR_2 src/Network)
set(CPP_DIR_3 src/Core)
set(CPP_DIR_4 src/Factories)
set(CPP_DIR_5 src/Sensors)
set(CPP_DIR_6 src/Utilities)
set(CPP_DIR_7 src/Sensors/lib)
set(CPP_DIR_8 src/Existences)
set(HEADER_DIR_1 src)
set(HEADER_DIR_2 src/Network)
set(HEADER_DIR_3 src/Core)
set(HEADER_DIR_4 src/Factories)
set(HEADER_DIR_5 src/Sensors)
set(HEADER_DIR_6 src/Utilities)
set(HEADER_DIR_7 src/Sensors/lib)
set(HEADER_DIR_8 src/Existences)
set(wiringPi_INCLUDE wiringPi/wiringPi)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CSV_PATH ../csv/include)

############## CMake Project ################
#        The main options of project        #
#############################################

if(WIN32)
    message(ERROR "Cannot build on windows targets")
endif()

project(${PROJECT_NAME} CXX)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: Use Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

############## Artefacts Output #################
# Defines outputs , depending Debug or Release. #
#################################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
else()
  set(CMAKE_BUILDTYPE "Release")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
endif()

message(STATUS "Building ${CMAKE_BUILD_TYPE} ")

################# Flags ################
# Defines Flags for Windows and Linux. #
########################################
find_library(wiringPi_LIB wiringPi)
find_library(sfml_system_LIB sfml-system)
find_library(sfml_graphics_LIB sfml-graphics)
find_library(sfml_network_LIB sfml-network)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(SFML 2.5.1 COMPONENTS system network graphics REQUIRED)

#include_directories(${wiringPi_INCLUDE})
include_directories(${OpenCV_INCLUDE_DIRS} ${CSV_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

################ Files ################
#   --   Add files to project.   --   #
#######################################

file(GLOB SRC_FILES
    ${CPP_DIR_1}/*.cpp
    ${CPP_DIR_2}/*.cpp
    ${CPP_DIR_3}/*.cpp
    ${CPP_DIR_4}/*.cpp
    ${CPP_DIR_5}/*.cpp
    ${CPP_DIR_6}/*.cpp
    ${CPP_DIR_7}/*.cpp
    ${CPP_DIR_8}/*.cpp
    ${HEADER_DIR_1}/*.h
    ${HEADER_DIR_2}/*.h
    ${HEADER_DIR_3}/*.h
    ${HEADER_DIR_4}/*.h
    ${HEADER_DIR_5}/*.h
    ${HEADER_DIR_6}/*.h
    ${HEADER_DIR_7}/*.h
    ${HEADER_DIR_8}/*.h
)

# Add executable to build.
add_executable(${PROJECT_NAME}
   ${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} ${wiringPi_LIB} ${sfml_network_LIB} ${sfml_system_LIB} Threads::Threads ${OpenCV_LIBS})
