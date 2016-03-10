cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Project build type")
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Install prefix")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_VERBOSE_MAKEFILE 1)

project(tktk C CXX)

option(TKTK_BUILD_MPOOL "Build tktk-mpool library (header-only, so install only)"  ON)
option(TKTK_BUILD_UTIL "Build tktk-util library (header-only, so install only)"  ON)
option(TKTK_BUILD_ECS "Build tktk-ecs library" ON)
option(TKTK_BUILD_PHASE "Build tktk-phase library" ON)
option(TKTK_BUILD_ASSET "Build tktk-asset library" ON)

option(TKTK_BUILD_UNIT_TESTS "Build unit tests for tktk libraries" ON)

option(TKTK_BUILD_ARCANOID "Build arcanoid demo" ON)

option(TKTK_LL_ENABLED "Build with LibLog" ON)
if(${TKTK_LL_ENABLED})
    add_definitions(-DTKTK_LL_ENABLED)
    set(TKTK_LL_LEVEL 5 CACHE STRING "LibLog severity level; 0: fatal, 1: error, 2: warning, 3: info, 4: debug, 5: trace" ) # option(TKTK_LL_LEVEL "LibLog severity level" 5)
    add_definitions(-DTKTK_LL_LEVEL=${TKTK_LL_LEVEL})
endif(${TKTK_LL_ENABLED})

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
include_directories(src)

# actualy all we need here is LibLog header
if(${TKTK_LL_ENABLED})
    include_directories(${CMAKE_SOURCE_DIR}/third-party/include)
endif(${TKTK_LL_ENABLED})


#
# Enable project folders
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")


add_subdirectory(src)

# Little help
message(STATUS "")
message(STATUS "Your options now:")
message(STATUS "    make         # To build")
message(STATUS "    make install # To install")
message(STATUS "")
