set(TKTK_LIB_TYPEC_HEADERS "")
set(TKTK_LIB_TYPEC_SOURCES "")
#=========================
# === tktk/mcecs group ===
# ========================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/mcecs/Config.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/mcecs/Ecs.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/tktk/mcecs/fake.cpp
)
source_group("tktk\\mcecs" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_MCECS_HEADERS ${TKTK_LIB_MCECS_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_MCECS_SOURCES ${TKTK_LIB_MCECS_SOURCES} ${GROUP_SOURCES})

add_library(tktk-mcecs-fake STATIC
	${TKTK_LIB_MCECS_HEADERS} ${TKTK_CONFIG_HEADERS}
	${TKTK_LIB_MCECS_SOURCES}
)
set_target_properties(tktk-mcecs-fake PROPERTIES LINKER_LANGUAGE CXX)

install(
    FILES ${TKTK_LIB_MCECS_HEADERS}
    DESTINATION include/tktk/mcecs
)
