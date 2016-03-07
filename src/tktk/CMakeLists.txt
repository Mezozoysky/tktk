
set(TKTK_CONFIG_HEADERS "")
#===================
# === tktk group ===
# ==================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/Config.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/LibLog.hpp
)
source_group("tktk" FILES ${GROUP_HEADERS})
set(TKTK_CONFIG_HEADERS ${TKTK_CONFIG_HEADERS} ${GROUP_HEADERS})

set(TKTK_LIB_UTIL_HEADERS "")
set(TKTK_LIB_UTIL_SOURCES "")
#========================
# === tktk/util group ===
# =======================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/util/TypeMap.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/util/Id64.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/util/MemoryPool.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/util/Signal.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/util/MemoryPool.cpp
)
source_group("tktk\\util" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_UTIL_HEADERS ${TKTK_LIB_UTIL_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_UTIL_SOURCES ${TKTK_LIB_UTIL_SOURCES} ${GROUP_SOURCES})


set(TKTK_LIB_ECS_HEADERS "")
set(TKTK_LIB_ECS_SOURCES "")
# ======================
# === tktk/ecs group ===
# ======================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/System.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Component.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Processor.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/System.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/Component.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/Processor.cpp
)
source_group("tktk\\ecs" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_ECS_HEADERS ${TKTK_LIB_ECS_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_ECS_SOURCES ${TKTK_LIB_ECS_SOURCES} ${GROUP_SOURCES})


set(TKTK_LIB_PHASE_HEADERS "")
set(TKTK_LIB_PHASE_SOURCES "")
# ========================
# === tktk/phase group ===
# ========================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/phase/IPhase.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/phase/PhaseManager.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/phase/PhaseManager.cpp
)
source_group("tktk\\phase" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_PHASE_HEADERS ${TKTK_LIB_PHASE_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_PHASE_SOURCES ${TKTK_LIB_PHASE_SOURCES} ${GROUP_SOURCES})

set(TKTK_LIB_ASSET_HEADERS "")
set(TKTK_LIB_ASSET_SOURCES "")
# =========================
# === tktk/resmgr group ===
# =========================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/asset/Config.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/asset/System.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/asset/Manager.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/asset/System.cpp
)
source_group("tktk\\asset" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_RESMGR_HEADERS ${TKTK_LIB_ASSET_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_RESMGR_SOURCES ${TKTK_LIB_ASSET_SOURCES} ${GROUP_SOURCES})

#
# Build static libraries!
#
add_library(tktk-util STATIC
   ${TKTK_LIB_UTIL_HEADERS} ${TKTK_CONFIG_HEADERS}
   ${TKTK_LIB_UTIL_SOURCES}
)
set_target_properties(tktk-util PROPERTIES LINKER_LANGUAGE CXX)

if(${TKTK_BUILD_ECS})
    add_library(tktk-ecs STATIC
        ${TKTK_LIB_ECS_HEADERS} ${TKTK_CONFIG_HEADERS}
        ${TKTK_LIB_ECS_SOURCES}
    )
endif(${TKTK_BUILD_ECS})

if(${TKTK_BUILD_PHASE})
    add_library(tktk-phase STATIC
        ${TKTK_LIB_PHASE_HEADERS} ${TKTK_CONFIG_HEADERS}
        ${TKTK_LIB_PHASE_SOURCES}
    )
endif(${TKTK_BUILD_PHASE})

if(${TKTK_BUILD_ASSET})
    add_library(tktk-asset STATIC
        ${TKTK_LIB_ASSET_HEADERS} ${TKTK_CONFIG_HEADERS}
        ${TKTK_LIB_ASSET_SOURCES}
    )
    set_target_properties(tktk-asset PROPERTIES LINKER_LANGUAGE CXX)
endif(${TKTK_BUILD_ASSET})
