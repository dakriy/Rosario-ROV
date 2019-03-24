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
set(CPP_DIR_7 imgui-sfml)
set(HEADER_DIR_1 src/Core)
set(HEADER_DIR_2 src/Factories)
set(HEADER_DIR_3 src/Frames)
set(HEADER_DIR_4 src)
set(HEADER_DIR_5 src/Interfaces)
set(HEADER_DIR_6 src/UI)
set(HEADER_DIR_7 imgui-sfml)
set(IMGUI_PATH imgui)
set(IMGUI_SFML_PATH imgui-sfml)

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
  set(SFML_LIBS "sfml-graphics-d.lib sfml-window-d.lib sfml-audio-d.lib sfml-network-d.lib sfml-system-d.lib")
else()
  set(CMAKE_BUILDTYPE "Release")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(SFML_LIBS "sfml-graphics.lib sfml-window.lib sfml-audio.lib sfml-network.lib sfml-system.lib")
  if(WIN32)
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
    find_package(SFML REQUIRED)
endif()

include_directories(${IMGUI_PATH} ${IMGUI_SFML_PATH})


if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MDd /Od /EHsc /std:c++latest")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /GL /Od /Oi /Gy /EHsc /std:c++latest /MD")
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
    ${CPP_DIR_7}/*.cpp
    ${HEADER_DIR_1}/*.h
    ${HEADER_DIR_2}/*.h
    ${HEADER_DIR_3}/*.h
    ${HEADER_DIR_4}/*.h
    ${HEADER_DIR_5}/*.h
    ${HEADER_DIR_6}/*.h
    ${HEADER_DIR_7}/*.h
)

# Add executable to build.
add_executable(${PROJECT_NAME} 
   ${SRC_FILES}
)


# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(STATUS "Copying resources to output directory ${CMAKE_CURRENT_BINARY_DIR}/Resources")

    # list of files for which we add a copy rule
    set(resources background.png lester.png ProggyClean.ttf)

    foreach(item IN LISTS resources)
        message(STATUS "Copying Resources/${item}")
        configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Resources/${item}" "${CMAKE_CURRENT_BINARY_DIR}/Resources/${item}" COPYONLY)
    endforeach()
endif()

if(MSVC)
   target_link_libraries(${PROJECT_NAME} ${SFML_LIBS} opengl32.lib freetype.lib gdi32.lib openal32.lib flac.lib vorbisenc.lib vorbisfile.lib vorbis.lib ogg.lib ws2_32.lib winmm.lib )
endif(MSVC)
