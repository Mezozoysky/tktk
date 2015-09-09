cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_INSTALL_PREFIX "/usr/local")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_MACOSX_RPATH 1)

project(tktk C CXX)

option(TKTK_BUILD_ECS "Build tktk-ecs library" ON)
option(TKTK_BUILD_PHASE "Build tktk-phase library" ON)
option(TKTK_BUILD_SCENE2D "Build tktk-scene2d library" ON)

option(TKTK_BUILD_UNIT_TESTS "Build unit tests for tktk libraries" ON)

option(TKTK_BUILD_ARCANOID "Build arcanoid demo" OFF)

#
# OS definitions
#
message("-- ${CMAKE_PROJECT_NAME} -- System name: ${CMAKE_SYSTEM_NAME}")
message("-- ${CMAKE_PROJECT_NAME} -- System version: ${CMAKE_SYSTEM_VERSION}")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(TKTK_OS_LINUX TRUE)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(TKTK_OS_OSX TRUE)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(TKTK_OS_WINDOWS TRUE)
else()
    message(FATAL_ERROR "UNSUPPORTED OS!")
endif()

#
# Compiler options
#
message("-- ${CMAKE_PROJECT_NAME} -- Compiler name: ${CMAKE_CXX_COMPILER_ID}")
message("-- ${CMAKE_PROJECT_NAME} -- Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
else()
    #TODO: investigate what to do on windows
endif()


#
# TKTK include dirs
#
include_directories(include)

#
# Enable project folders
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")


add_subdirectory(src)
