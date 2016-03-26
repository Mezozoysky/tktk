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

include_directories(${CMAKE_SOURCE_DIR}/third-party/include)

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
else(SDL2_FOUND)
    message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No SDL2 found!")
endif(SDL2_FOUND)

find_package(SDL2_image REQUIRED)
if(SDL2_IMAGE_FOUND)
    include_directories(${SDL2_IMAGE_INCLUDE_DIR})
else(SDL2_IMAGE_FOUND)
    message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No SDL2_image found!")
endif(SDL2_IMAGE_FOUND)

find_package(SDL2_ttf REQUIRED)
if(SDL2_TTF_FOUND)
    include_directories(${SDL2_TTF_INCLUDE_DIR})
else(SDL2_TTF_FOUND)
    message(FATAL_ERROR "-- ${CMAKE_PROJECT_NAME} -- No SDL2_ttf found!")
endif(SDL2_TTF_FOUND)

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
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/Image.hpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/RectShape.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/Transform.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/Image.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/comp/RectShape.cpp
)
source_group("arcanoid\\comp" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(ARCANOID_HEADERS ${ARCANOID_HEADERS} ${GROUP_HEADERS})
set(ARCANOID_SOURCES ${ARCANOID_SOURCES} ${GROUP_SOURCES})

# ==============================
# ==== arcanoid/asset group ====
# ==============================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/src/arcanoid/asset/TextureMgr.hpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/asset/JsonMgr.hpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/asset/FontMgr.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arcanoid/asset/TextureMgr.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/asset/JsonMgr.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/asset/FontMgr.cpp
)
source_group("arcanoid\\asset" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(ARCANOID_HEADERS ${ARCANOID_HEADERS} ${GROUP_HEADERS})
set(ARCANOID_SOURCES ${ARCANOID_SOURCES} ${GROUP_SOURCES})

# ===============================
# ==== arcanoid/render group ====
# ===============================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/src/arcanoid/render/Texture.hpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/render/Font.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arcanoid/render/Texture.cpp
    ${CMAKE_SOURCE_DIR}/src/arcanoid/render/Font.cpp
)
source_group("arcanoid\\asset" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(ARCANOID_HEADERS ${ARCANOID_HEADERS} ${GROUP_HEADERS})
set(ARCANOID_SOURCES ${ARCANOID_SOURCES} ${GROUP_SOURCES})

# =============================
# ==== arcanoid/json group ====
# =============================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/src/arcanoid/json/JSON.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arcanoid/json/JSON.cpp
)
source_group("arcanoid\\json" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(ARCANOID_HEADERS ${ARCANOID_HEADERS} ${GROUP_HEADERS})
set(ARCANOID_SOURCES ${ARCANOID_SOURCES} ${GROUP_SOURCES})


add_executable(arcanoid WIN32 MACOSX_BUNDLE
    ${ARCANOID_HEADERS}
    ${ARCANOID_SOURCES}
)
set_target_properties(arcanoid PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(arcanoid tktk-ecs tktk-asset)
target_link_libraries(arcanoid ${SDL2_TTF_LIBRARY})
target_link_libraries(arcanoid ${SDL2_IMAGE_LIBRARY})
target_link_libraries(arcanoid ${SDL2_LIBRARY})
target_link_libraries(arcanoid ${OPENGL_LIBRARIES})
target_link_libraries(arcanoid ${TKTK_OS_SPECIFIC_LIBS})

add_custom_command(
    TARGET arcanoid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets/arcanoid $<TARGET_FILE_DIR:arcanoid>
)
