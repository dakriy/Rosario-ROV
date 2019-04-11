cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
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
    if(WIN32)
        set(SFML_LIBS "sfml-network-d.lib sfml-system-d.lib")
    endif()
else()
  set(CMAKE_BUILDTYPE "Release")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  if(WIN32)
      set(SFML_LIBS "sfml-network.lib sfml-system.lib")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  endif()
endif()

message(STATUS "Building ${CMAKE_BUILD_TYPE} ")

################# Flags ################
# Defines Flags for Windows and Linux. #
########################################

if(WIN32)
    set(SFML_INCLUDE "$ENV{SFML_INCLUDE}")
    set(SFML_LIB "$ENV{SFML_LIB}")

    if (SFML_INCLUDE STREQUAL "")
        message(FATAL_ERROR "Please define the SFML_INCLUDE environment variable to the sfml include path")
    endif()
    if (SFML_LIB STREQUAL "")
        message(FATAL_ERROR "Please define the SFML_LIB environment variable to the sfml lib path")
    endif()

    link_directories(${SFML_LIB})
    include_directories(${SFML_INCLUDE})
else()
		find_package(SFML 2.5.1 COMPONENTS system network REQUIRED)
        find_library(FOO_LIB foo)
endif()


if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MDd /Od /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /GL /Od /Oi /Gy /EHsc /MD")
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
)

# Add executable to build.
add_executable(${PROJECT_NAME}
   ${SRC_FILES}
)

if(MSVC)
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBS} freetype.lib gdi32.lib openal32.lib flac.lib vorbisenc.lib vorbisfile.lib vorbis.lib ogg.lib ws2_32.lib winmm.lib)
else()
    target_link_libraries(${PROJECT_NAME} sfml-system sfml-network)
endif()
