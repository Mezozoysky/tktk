include_directories(${CMAKE_SOURCE_DIR}/third-party/include)

set(TKTK_UT_HEADERS "")
set(TKTK_UT_SOURCES "")
#=========================
# === tktk/tests group ===
# ========================
set(GROUP_HEADERS
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tests/main.cpp
)
source_group("tktk\\tests" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_UT_HEADERS ${TKTK_LIB_UTIL_HEADERS} ${GROUP_HEADERS})
set(TKTK_UT_SOURCES ${TKTK_LIB_UTIL_SOURCES} ${GROUP_SOURCES})

add_executable(tktk-tests WIN32
    ${TKTK_UT_HEADERS}
    ${TKTK_UT_SOURCES}
)
set_target_properties(tktk-tests PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(tktk-tests tktk-ecs tktk-util)
