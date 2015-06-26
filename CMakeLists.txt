cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

macro(macro_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

#
# Options which should be declared before the project() instruction
#
macro_set_option(CMAKE_BUILD_TYPE "Debug" STRING "Choose the type of build (Debug or Release)")
macro_set_option(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" PATH "Directory where to install targets")


project(tktk C CXX)

#
# Other options
#
macro_set_option(TKTK_BUILD_SHARED FALSE BOOL "TRUE to build TKTK as shared libraries, FALSE to build as static")
macro_set_option(TKTK_USE_SHARED_DEPS FALSE BOOL "TRUE to use shared SFML, etc (with FindSFML, etc modules)")

macro_set_option(TKTK_BUILD_DEMO FALSE BOOL "TRUE to build TKTK as shared libraries, FALSE to build as static")

#
# Shared/static lib settings
#
if(NOT TKTK_BUILD_SHARED)
    message("-- ${CMAKE_PROJECT_NAME} -- Library build type: static")
    add_definitions(-DTKTK_STATIC)
else()
    message("-- ${CMAKE_PROJECT_NAME} -- Library build type: shared")
    add_definitions(-DTKTK_SHARED)
endif()

#
# Shared/static deps settings
#
if(NOT TKTK_USE_SHARED_DEPS)
    set(SFML_STATIC_LIBRARIES TRUE)
else(NOT TKTK_USE_SHARED_DEPS)
    set(SFML_STATIC_LIBRARIES FALSE)
endif(NOT TKTK_USE_SHARED_DEPS)


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
  #do nothing, msvc can 11 out of the box
endif()

#
# Modules path
#
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

#
# Targets/names
#
set(TKTK_CORE_NAME tktk)
set(TKTK_DEMO_NAME tktk-demo)

#
# OS-specific libs
#
set(TKTK_LINUX_LIBS "")
if(TKTK_OS_LINUX)
    if(NOT TKTK_BUILD_SHARED)
        find_library(X11XCB_LIB X11-xcb)
        find_library(XCB_LIB xcb)
        find_library(PTHREAD_LIB pthread)
        set(TKTK_LINUX_LIBS ${TKTK_LINUX_LIBS}
            ${X11XCB_LIB}
            ${XCB_IMAGE_LIBRARY}
            ${XCB_RANDR_LIBRARY}
            ${XCB_LIB}
            ${UDEV_LIBRARIES}
            ${PTHREAD_LIB}
        )
    endif(NOT TKTK_BUILD_SHARED)
endif(TKTK_OS_LINUX)

set(TKTK_OSX_LIBS "")
if(${TKTK_OS_OSX})
  find_library(CF_LIB NAMES CoreFoundation)
  find_library(CARBON_LIB Carbon)
  find_library(COCOA_LIB Cocoa)
  find_library(IOKIT_LIB IOKit)
  set(TKTK_OSX_LIBS ${CF_LIB} ${CARBON_LIB} ${COCOA_LIB} ${IOKIT_LIB})
endif()

set(TKTK_WINDOWS_LIBS "")
if(${TKTK_OS_WINDOWS})
endif()

#
# Packages & dependencies
#
set(TKTK_DEP_LIBS "")
set(TKTK_DEMO_DEP_LIBS "")

find_package(SFML 2.2 COMPONENTS audio network graphics window system  REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
else(SFML_FOUND)
    message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No SFML found!")
endif(SFML_FOUND)

find_package(OpenGL)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
else(OPENGL_FOUND)
    message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No OpenGL found!")
endif(OPENGL_FOUND)

if(TKTK_USE_SHARED_DEPS)
    set(TKTK_DEP_LIBS ${TKTK_DEP_LIBS}
        #nothing
    )
    set(TKTK_DEMO_DEP_LIBS ${TKTK_DEMO_DEP_LIBS}
        ${SFML_LIBRARIES}
        ${OPENGL_LIBRARIES}
    )
else(TKTK_USE_SHARED_DEPS)
    set(TKTK_DEP_LIBS ${TKTK_DEP_LIBS}
        #nothing
    )
    set(TKTK_DEMO_DEP_LIBS ${TKTK_DEMO_DEP_LIBS}
        ${SFML_LIBRARIES}
        ${JPEG_LIBRARY}
        ${OPENGL_LIBRARIES}
    )
endif(TKTK_USE_SHARED_DEPS)


#
# TKTK include dirs
#
include_directories(include)
include_directories(src)


#
# Enable project folders
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")


set(HEADERS "")
set(SOURCES "")

# ==================
# === tktk group ===
# ==================
set(GROUP_HEADERS
  include/${TKTK_CORE_NAME}/tktk.h
)
set(GROUP_SOURCES
  src/${TKTK_CORE_NAME}/tktk.cpp
)
source_group("tktk" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(HEADERS ${HEADERS} ${GROUP_HEADERS})
set(SOURCES ${SOURCES} ${GROUP_SOURCES})

#
# Build the library!
#
if(TKTK_BUILD_SHARED)
    add_library(${TKTK_CORE_NAME} SHARED
        ${HEADERS}
        ${SOURCES}
    )
else(TKTK_BUILD_SHARED) #static
    # message(FATAL_ERROR "STATIC LIB NOT IMPLEMENTED")
    add_library(${TKTK_CORE_NAME} STATIC
        ${HEADERS}
        ${SOURCES}
    )
endif(TKTK_BUILD_SHARED)

message("** TKTK_DEP_LIBS: ${TKTK_DEP_LIBS}")
target_link_libraries(${TKTK_CORE_NAME} ${TKTK_DEP_LIBS})


#
# Build demo app
#
set(HEADERS "")
set(SOURCES "")

# =========================
# ==== tktk-demo group ====
# =========================
set(GROUP_HEADERS
)
set(GROUP_SOURCES
    ${TKTK_DEMO_NAME}/src/${TKTK_DEMO_NAME}/main.cpp
)
source_group("tktk-demo" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(HEADERS ${HEADERS} ${GROUP_HEADERS})
set(SOURCES ${SOURCES} ${GROUP_SOURCES})


add_executable(${TKTK_DEMO_NAME} WIN32 #MACOSX_BUNDLE
    ${HEADERS}
    ${SOURCES}
)
set_target_properties(${TKTK_DEMO_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${TKTK_DEMO_NAME} ${TKTK_CORE_NAME})
target_link_libraries(${TKTK_DEMO_NAME} ${TKTK_DEMO_DEP_LIBS})
target_link_libraries(${TKTK_DEMO_NAME} ${TKTK_LINUX_LIBS} ${TKTK_OSX_LIBS} ${TKTK_WINDOWS_LIBS})