set(TKTK_LIB_ECS_HEADERS "")
set(TKTK_LIB_ECS_SOURCES "")
# ==========================
# === tktk/ecs group ===
# ==========================
set(GROUP_HEADERS
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Entity.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/EntityManager.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/ComponentBase.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/ProcessorBase.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/Processor.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/ecs/System.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/ComponentBase.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/ProcessorBase.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/Processor.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/ecs/System.cpp
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
    ${CMAKE_SOURCE_DIR}/include/tktk/scene2d/Node.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/scene2d/Scene.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/scene2d/SceneObject.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/scene2d/ComponentBase.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/scene2d/Node.cpp
#    ${CMAKE_SOURCE_DIR}/src/tktk/scene2d/Scene.cpp
#    ${CMAKE_SOURCE_DIR}/src/tktk/scene2d/SceneObject.cpp
    ${CMAKE_SOURCE_DIR}/src/tktk/scene2d/ComponentBase.cpp
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
    ${CMAKE_SOURCE_DIR}/include/tktk/phase/IPhase.hpp
    ${CMAKE_SOURCE_DIR}/include/tktk/phase/PhaseManager.hpp
)
set(GROUP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/tktk/phase/PhaseManager.cpp
)
source_group("tktk\\phase" FILES ${GROUP_HEADERS} ${GROUP_SOURCES})
set(TKTK_LIB_PHASE_HEADERS ${TKTK_LIB_PHASE_HEADERS} ${GROUP_HEADERS})
set(TKTK_LIB_PHASE_SOURCES ${TKTK_LIB_PHASE_SOURCES} ${GROUP_SOURCES})

#
# Build shared libraries!
#
add_library(tktk-ecs SHARED
    ${TKTK_LIB_ECS_HEADERS}
    ${TKTK_LIB_ECS_SOURCES}
)
add_library(tktk-scene2d SHARED
    ${TKTK_LIB_SCENE2D_HEADERS}
    ${TKTK_LIB_SCENE2D_SOURCES}
)
add_library(tktk-phase SHARED
    ${TKTK_LIB_PHASE_HEADERS}
    ${TKTK_LIB_PHASE_SOURCES}
)

#
# Build static libraries!
#
add_library(tktk-ecs-s STATIC
    ${TKTK_LIB_ECS_HEADERS}
    ${TKTK_LIB_ECS_SOURCES}
)
add_library(tktk-scene2d-s STATIC
    ${TKTK_LIB_SCENE2D_HEADERS}
    ${TKTK_LIB_SCENE2D_SOURCES}
)
add_library(tktk-phase-s STATIC
    ${TKTK_LIB_PHASE_HEADERS}
    ${TKTK_LIB_PHASE_SOURCES}
)
