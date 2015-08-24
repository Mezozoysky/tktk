cmake_minimum_required(VERSION 2.8.4)

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

macro_set_option(TKTK_BUILD_DEMO FALSE BOOL "TRUE to build TKTK demo application")
macro_set_option(TKTK_BUILD_SCENE2D_DEMO FALSE BOOL "TRUE to build TKTK scene2d demo application")

#
# Shared/static lib settings
#
if(NOT TKTK_BUILD_SHARED) #static!
    message("-- ${CMAKE_PROJECT_NAME} -- Library build type: static")
    set(SFML_STATIC_LIBRARIES TRUE)
    add_definitions(-DTKTK_STATIC)
else() #shared!
    message("-- ${CMAKE_PROJECT_NAME} -- Library build type: shared")
    set(SFML_STATIC_LIBRARIES FALSE)
    add_definitions(-DTKTK_SHARED)
endif()

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
set(TKTK_LIB_ECS tktk-ecs)
set(TKTK_LIB_SCENE2D tktk-scene2d)
set(TKTK_LIB_PHASE tktk-phase)
set(TKTK_APP_DEMO tktk-demo)
set(ARCANOID arcanoid)

#
# OS-specific libs
#
set(TKTK_APP_LINUX_LIBS "")
if(TKTK_OS_LINUX)
    find_library(PTHREAD_LIBRARY pthread)
    if(TKTK_BUILD_SHARED)
        set(TKTK_APP_LINUX_LIBS ${TKTK_APP_LINUX_LIBS}
            ${XCB_IMAGE_LIBRARY}
            ${XCB_RANDR_LIBRARY}
            ${X11_XCB_LIBRARY}
            ${LIBXCB_LIBRARIES}
            ${UDEV_LIBRARIES}
            ${PTHREAD_LIBRARY}
        )
    else(TKTK_BUILD_SHARED)
        set(TKTK_APP_LINUX_LIBS ${TKTK_APP_LINUX_LIBS}
            ${XCB_IMAGE_LIBRARY}
            ${XCB_RANDR_LIBRARY}
            ${X11_XCB_LIBRARY}
            ${LIBXCB_LIBRARIES}
            ${UDEV_LIBRARIES}
            ${PTHREAD_LIBRARY}
        )
    endif(TKTK_BUILD_SHARED)
endif(TKTK_OS_LINUX)

set(TKTK_APP_OSX_LIBS "")
if(${TKTK_APP_OS_OSX})
  find_library(CF_LIB NAMES CoreFoundation)
  find_library(CARBON_LIB Carbon)
  find_library(COCOA_LIB Cocoa)
  find_library(IOKIT_LIB IOKit)
  set(TKTK_APP_OSX_LIBS ${CF_LIB} ${CARBON_LIB} ${COCOA_LIB} ${IOKIT_LIB})
endif()

set(TKTK_APP_WINDOWS_LIBS "")
if(${TKTK_OS_WINDOWS})
endif()

#
# Packages & dependencies
#
set(TKTK_LIB_ECS_DEPS "")
set(TKTK_LIB_SCENE2D_DEPS "")
set(TKTK_LIB_PHASE_DEPS "")
set(TKTK_APP_DEMO_DEPS "")

find_package(SFML 2.2 COMPONENTS audio network graphics window system  REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
else(SFML_FOUND)
    if(TKTK_BUILD_DEMO OR TKTK_BUILD_SCENE2D_DEMO)
        message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No SFML found!")
    endif(TKTK_BUILD_DEMO OR TKTK_BUILD_SCENE2D_DEMO)
endif(SFML_FOUND)

find_package(OpenGL)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
else(OPENGL_FOUND)
    message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No OpenGL found!")
endif(OPENGL_FOUND)

if(TKTK_USE_SHARED_DEPS)
    set(TKTK_APP_DEMO_DEPS ${TKTK_APP_DEMO_DEPS}
        ${SFML_LIBRARIES}
        ${OPENGL_LIBRARIES}
    )
else(TKTK_USE_SHARED_DEPS)
    set(TKTK_APP_DEMO_DEPS ${TKTK_APP_DEMO_DEPS}
        ${SFML_LIBRARIES}
        ${FREETYPE_LIBRARY}
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


set(TKTK_LIB_ECS_HEADERS "")
set(TKTK_LIB_ECS_SOURCES "")
# ==========================
# === tktk/ecs group ===
# ==========================
set(GROUP_HEADERS
    include/tktk/ecs/Entity.hpp
    include/tktk/ecs/EntityManager.hpp
    include/tktk/ecs/ComponentBase.hpp
    include/tktk/ecs/ProcessorBase.hpp
    include/tktk/ecs/Processor.hpp
    include/tktk/ecs/System.hpp
)
set(GROUP_SOURCES
    src/tktk/ecs/Entity.cpp
    src/tktk/ecs/EntityManager.cpp
    src/tktk/ecs/ComponentBase.cpp
    src/tktk/ecs/ProcessorBase.cpp
    src/tktk/ecs/Processor.cpp
    src/tktk/ecs/System.cpp
)
source_group("tktk\\ecs" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_ECS_HEADERS ${TKTK_LIB_ECS_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_ECS_SOURCES ${TKTK_LIB_ECS_SOURCES} ${GROUP_SOURCES})


set(TKTK_LIB_SCENE2D_HEADERS "")
set(TKTK_LIB_SCENE2D_SOURCES "")
# ==========================
# === tktk/scene2d group ===
# ==========================
set(GROUP_HEADERS
    include/tktk/scene2d/Node.hpp
    include/tktk/scene2d/Scene.hpp
    include/tktk/scene2d/SceneObject.hpp
    include/tktk/scene2d/ComponentBase.hpp
)
set(GROUP_SOURCES
    src/tktk/scene2d/Node.cpp
#    src/tktk/scene2d/Scene.cpp
#    src/tktk/scene2d/SceneObject.cpp
    src/tktk/scene2d/ComponentBase.cpp
)
source_group("tktk\\scene2d" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_SCENE2D_HEADERS ${TKTK_LIB_SCENE2D_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_SCENE2D_SOURCES ${TKTK_LIB_SCENE2D_SOURCES} ${GROUP_SOURCES})


set(TKTK_LIB_PHASE_HEADERS "")
set(TKTK_LIB_PHASE_SOURCES "")
# ========================
# === tktk/phase group ===
# ========================
set(GROUP_HEADERS
    include/tktk/phase/IPhase.hpp
    include/tktk/phase/PhaseManager.hpp
)
set(GROUP_SOURCES
    src/tktk/phase/PhaseManager.cpp
)
source_group("tktk\\phase" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_PHASE_HEADERS ${TKTK_LIB_PHASE_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_PHASE_SOURCES ${TKTK_LIB_PHASE_SOURCES} ${GROUP_SOURCES})

#
# Build the library!
#
if(TKTK_BUILD_SHARED)
    add_library(${TKTK_LIB_ECS} SHARED
        ${TKTK_LIB_ECS_HEADERS}
        ${TKTK_LIB_ECS_SOURCES}
    )
    add_library(${TKTK_LIB_SCENE2D} SHARED
        ${TKTK_LIB_SCENE2D_HEADERS}
        ${TKTK_LIB_SCENE2D_SOURCES}
    )
    add_library(${TKTK_LIB_PHASE} SHARED
        ${TKTK_LIB_PHASE_HEADERS}
        ${TKTK_LIB_PHASE_SOURCES}
    )
else(TKTK_BUILD_SHARED) #static
    add_library(${TKTK_LIB_ECS} STATIC
        ${TKTK_LIB_ECS_HEADERS}
        ${TKTK_LIB_ECS_SOURCES}
    )
    add_library(${TKTK_LIB_SCENE2D} STATIC
        ${TKTK_LIB_SCENE2D_HEADERS}
        ${TKTK_LIB_SCENE2D_SOURCES}
    )
    add_library(${TKTK_LIB_PHASE} STATIC
        ${TKTK_LIB_PHASE_HEADERS}
        ${TKTK_LIB_PHASE_SOURCES}
    )
endif(TKTK_BUILD_SHARED)

target_link_libraries(${TKTK_LIB_ECS} ${TKTK_LIB_ECS_DEPS})
target_link_libraries(${TKTK_LIB_SCENE2D} ${TKTK_LIB_SCENE2D_DEPS})
target_link_libraries(${TKTK_LIB_PHASE} ${TKTK_LIB_PHASE_DEPS})


#
# Build demo app
#
set(TKTK_APP_DEMO_HEADERS "")
set(TKTK_APP_DEMO_SOURCES "")

# =========================
# ==== tktk-demo group ====
# =========================
set(GROUP_HEADERS
)
set(GROUP_SOURCES
    ${TKTK_APP_DEMO}/src/${TKTK_APP_DEMO}/main.cpp
)
source_group("tktk-demo" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_APP_DEMO_HEADERS ${TKTK_APP_DEMO_HEADERS} ${GROUP_HEADERS})
set(TKTK_APP_DEMO_SOURCES ${TKTK_APP_DEMO_SOURCES} ${GROUP_SOURCES})


add_executable(${TKTK_APP_DEMO} WIN32 #MACOSX_BUNDLE
    ${TKTK_APP_DEMO_HEADERS}
    ${TKTK_APP_DEMO_SOURCES}
)
set_target_properties(${TKTK_APP_DEMO_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${TKTK_APP_DEMO} ${TKTK_LIB_SCENE2D})
target_link_libraries(${TKTK_APP_DEMO} ${TKTK_LIB_PHASE})
target_link_libraries(${TKTK_APP_DEMO} ${TKTK_APP_DEMO_DEPS})
target_link_libraries(${TKTK_APP_DEMO} ${TKTK_APP_LINUX_LIBS} ${TKTK_APP_OSX_LIBS} ${TKTK_APP_WINDOWS_LIBS})

#
# Build arcanoid
#
set(ARCANOID_HEADERS "")
set(ARCANOID_SOURCES "")

# ========================
# ==== arcanoid group ====
# ========================
set(GROUP_HEADERS
    src/${ARCANOID}/Game.hpp
)
set(GROUP_SOURCES
    src/${ARCANOID}/Game.cpp
    src/${ARCANOID}/main.cpp
)
source_group("arcanoid" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(ARCANOID_HEADERS ${ARCANOID_HEADERS} ${GROUP_HEADERS})
set(ARCANOID_SOURCES ${ARCANOID_SOURCES} ${GROUP_SOURCES})

# =============================
# ==== arcanoid/comp group ====
# =============================
set(GROUP_HEADERS
    src/${ARCANOID}/comp/Transform.hpp
)
set(GROUP_SOURCES
    src/${ARCANOID}/comp/Transform.cpp
)
source_group("arcanoid\\comp" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(ARCANOID_HEADERS ${ARCANOID_HEADERS} ${GROUP_HEADERS})
set(ARCANOID_SOURCES ${ARCANOID_SOURCES} ${GROUP_SOURCES})


add_executable(${ARCANOID} WIN32 #MACOSX_BUNDLE
    ${ARCANOID_HEADERS}
    ${ARCANOID_SOURCES}
)
set_target_properties(${ARCANOID_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${ARCANOID} ${TKTK_LIB_ECS})
target_link_libraries(${ARCANOID} ${TKTK_APP_DEMO_DEPS})
target_link_libraries(${ARCANOID} ${TKTK_APP_LINUX_LIBS} ${TKTK_APP_OSX_LIBS} ${TKTK_APP_WINDOWS_LIBS})
