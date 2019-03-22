cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

################### Variables. ####################
# Change if you want modify path or other values. #
###################################################

set(PROJECT_NAME ROVController)
# Output Variables
set(OUTPUT_DEBUG Debug/bin)
set(OUTPUT_RELEASE Release/bin)
# Folders files
set(CPP_DIR_1 imgui)
set(CPP_DIR_2 src/Core)
set(CPP_DIR_3 src/Factories)
set(CPP_DIR_4 src/Frames)
set(CPP_DIR_5 src)
set(CPP_DIR_6 src/UI)
set(HEADER_DIR_1 src/Core)
set(HEADER_DIR_2 src/Factories)
set(HEADER_DIR_3 src/Frames)
set(HEADER_DIR_4 src)
set(HEADER_DIR_5 src/Interfaces)
set(HEADER_DIR_6 src/UI)

############## CMake Project ################
#        The main options of project        #
#############################################

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
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
endif()

################# Flags ################
# Defines Flags for Windows and Linux. #
########################################

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MD /Od /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /GL /Od /Oi /Gy /EHsc")
endif(MSVC)
if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
   endif()
endif(NOT MSVC)

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
    ${HEADER_DIR_1}/*.h
    ${HEADER_DIR_2}/*.h
    ${HEADER_DIR_3}/*.h
    ${HEADER_DIR_4}/*.h
    ${HEADER_DIR_5}/*.h
    ${HEADER_DIR_6}/*.h
)

# Add executable to build.
add_executable(${PROJECT_NAME} 
   ${SRC_FILES}
)

if(MSVC)
   target_link_libraries(${PROJECT_NAME} sfml-graphics-d.lib sfml-window-d.lib sfml-audio-d.lib sfml-network-d.lib sfml-system-d.lib opengl32.lib freetype.lib gdi32.lib openal32.lib flac.lib vorbisenc.lib vorbisfile.lib vorbis.lib ogg.lib ws2_32.lib winmm.lib )
endif(MSVC)
