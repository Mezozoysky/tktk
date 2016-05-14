set(TKTK_BUILD_AND_EXPORT_LIB_TYPEC ON)
if(${TKTK_BUILD_AND_EXPORT_LIB_TYPEC})
    add_subdirectory(typec)
endif(${TKTK_BUILD_AND_EXPORT_LIB_TYPEC})

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
    ${CMAKE_SOURCE_DIR}/include/tktk/util/Config.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/util/Signal.hpp
)
set(GROUP_SOURCES
)
source_group("tktk\\util" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_UTIL_HEADERS ${TKTK_LIB_UTIL_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_UTIL_SOURCES ${TKTK_LIB_UTIL_SOURCES} ${GROUP_SOURCES})


set(TKTK_LIB_MPOOL_HEADERS "")
set(TKTK_LIB_MPOOL_SOURCES "")
#=========================
# === tktk/mpool group ===
# ========================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/mpool/Config.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/mpool/Id64.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/mpool/AMemoryPool.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/mpool/MemoryPool.hpp
)
set(GROUP_SOURCES
)
source_group("tktk\\mpool" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_MPOOL_HEADERS ${TKTK_LIB_MPOOL_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_MPOOL_SOURCES ${TKTK_LIB_MPOOL_SOURCES} ${GROUP_SOURCES})


set(TKTK_LIB_ECS_HEADERS "")
set(TKTK_LIB_ECS_SOURCES "")
# ======================
# === tktk/ecs group ===
# ======================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Config.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/System.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Component.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Processor.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/SystemB.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Mgr.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/MPoolBasedMgr.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/EntityMgr.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/CompMgr.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Ecs.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/System.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/Component.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/Processor.cpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Ecs.hpp
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
# ========================
# === tktk/asset group ===
# ========================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/asset/Config.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/asset/System.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/asset/Manager.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/asset/System.cpp
)
source_group("tktk\\asset" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_ASSET_HEADERS ${TKTK_LIB_ASSET_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_ASSET_SOURCES ${TKTK_LIB_ASSET_SOURCES} ${GROUP_SOURCES})


#
# Build and install libraries!
#

install(
    FILES ${TKTK_CONFIG_HEADERS}
    DESTINATION include/tktk
)

if(${TKTK_BUILD_MPOOL})
    install(
        FILES ${TKTK_LIB_MPOOL_HEADERS}
        DESTINATION include/tktk/mpool
    )
endif(${TKTK_BUILD_MPOOL})

if(${TKTK_BUILD_UTIL})
    install(
        FILES ${TKTK_LIB_UTIL_HEADERS}
        DESTINATION include/tktk/util
    )
endif(${TKTK_BUILD_UTIL})

if(${TKTK_BUILD_ECS})
    add_library(tktk-ecs STATIC
        ${TKTK_LIB_ECS_HEADERS} ${TKTK_CONFIG_HEADERS}
        ${TKTK_LIB_ECS_SOURCES}
    )

    install(
        FILES ${TKTK_LIB_ECS_HEADERS}
        DESTINATION include/tktk/ecs
    )
endif(${TKTK_BUILD_ECS})

if(${TKTK_BUILD_PHASE})
    add_library(tktk-phase STATIC
        ${TKTK_LIB_PHASE_HEADERS} ${TKTK_CONFIG_HEADERS}
        ${TKTK_LIB_PHASE_SOURCES}
    )

    install(
        FILES ${TKTK_LIB_PHASE_HEADERS}
        DESTINATION include/tktk/phase
    )
endif(${TKTK_BUILD_PHASE})

if(${TKTK_BUILD_ASSET})
    add_library(tktk-asset STATIC
        ${TKTK_LIB_ASSET_HEADERS} ${TKTK_CONFIG_HEADERS}
        ${TKTK_LIB_ASSET_SOURCES}
    )
    set_target_properties(tktk-asset PROPERTIES LINKER_LANGUAGE CXX)
    install(
        FILES ${TKTK_LIB_ASSET_HEADERS}
        DESTINATION include/tktk/asset
    )
endif(${TKTK_BUILD_ASSET})
