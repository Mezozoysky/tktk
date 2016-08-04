set(TKTK_LIB_TYPEC_HEADERS "")
set(TKTK_LIB_TYPEC_SOURCES "")
#===========================
# === tktk/typec group ===
# ==========================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/typec/Config.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/typec/TypeMap.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/typec/TypeMask.hpp
)
set(GROUP_SOURCES
	${CMAKE_SOURCE_DIR}/src/tktk/typec/fake.cpp
)
source_group("tktk\\typec" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_TYPEC_HEADERS ${TKTK_LIB_TYPEC_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_TYPEC_SOURCES ${TKTK_LIB_TYPEC_SOURCES} ${GROUP_SOURCES})

add_library(tktk-typec-fake STATIC
	${TKTK_LIB_TYPEC_HEADERS} ${TKTK_CONFIG_HEADERS}
	${TKTK_LIB_TYPEC_SOURCES}
)
set_target_properties(tktk-typec-fake PROPERTIES LINKER_LANGUAGE CXX)

install(
    FILES ${TKTK_LIB_TYPEC_HEADERS}
    DESTINATION include/tktk/typec
)
