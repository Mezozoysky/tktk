# ARCANOID

#
# OS-specific libs
#
set(TKTK_OS_SPECIFIC_LIBS "")
if(TKTK_OS_LINUX)
    # find_library(PTHREAD_LIBRARY pthread)
    set(TKTK_OS_SPECIFIC_LIBS ${TKTK_OS_SPECIFIC_LIBS}
        # ${XCB_IMAGE_LIBRARY}
        # ${XCB_RANDR_LIBRARY}
        # ${X11_XCB_LIBRARY}
        # ${LIBXCB_LIBRARIES}
        # ${UDEV_LIBRARIES}
        # ${PTHREAD_LIBRARY}
    )
endif(TKTK_OS_LINUX)

if(TKTK_OS_OSX)
    find_library(CORE_LIB NAMES CoreFoundation)
    find_library(COCOA_LIB Cocoa)
    find_library(IOKIT_LIB IOKit)
    set(TKTK_OS_SPECIFIC_LIBS ${TKTK_OS_SPECIFIC_LIBS}
        ${CORE_LIB}
        ${COCOA_LIB}
        ${IOKIT_LIB}
    )
endif(TKTK_OS_OSX)

if(TKTK_OS_WINDOWS)
    set(TKTK_OS_SPECIFIC_LIBS ${TKTK_OS_SPECIFIC_LIBS}
        #TODO: investigation what to do on windows.
    )
endif(TKTK_OS_WINDOWS)

#
# Packages & dependencies
#

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
else(SDL2_FOUND)
    message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No SDL2 found!")
endif(SDL2_FOUND)

find_package(OpenGL)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
else(OPENGL_FOUND)
    message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No OpenGL found!")
endif(OPENGL_FOUND)


#
# Build arcanoid
#
set(ARCANOID_HEADERS "")
set(ARCANOID_SOURCES "")

# ========================
# ==== arcanoid group ====
# ========================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/src/arcanoid/ECS.hpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/Game.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arcanoid/ECS.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/Game.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/main.cpp
)
source_group("arcanoid" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(ARCANOID_HEADERS ${ARCANOID_HEADERS} ${GROUP_HEADERS})
set(ARCANOID_SOURCES ${ARCANOID_SOURCES} ${GROUP_SOURCES})

# =============================
# ==== arcanoid/comp group ====
# =============================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/Transform.hpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/Renderer.hpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/StaticSprite.hpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/SpriteSheet.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/Transform.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/StaticSprite.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/SpriteSheet.cpp
)
source_group("arcanoid\\comp" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(ARCANOID_HEADERS ${ARCANOID_HEADERS} ${GROUP_HEADERS})
set(ARCANOID_SOURCES ${ARCANOID_SOURCES} ${GROUP_SOURCES})


add_executable(arcanoid WIN32 MACOSX_BUNDLE
    ${ARCANOID_HEADERS}
    ${ARCANOID_SOURCES}
)
set_target_properties(arcanoid PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(arcanoid tktk-ecs)
target_link_libraries(arcanoid ${SDL2_LIBRARY})
target_link_libraries(arcanoid ${OPENGL_LIBRARIES})
target_link_libraries(arcanoid ${TKTK_OS_SPECIFIC_LIBS})
