# ─── 核心引擎库：World / GridModel / Engine / 组件元模型 ───
# 全项目唯一 include 根目录的地方（${CMAKE_SOURCE_DIR}），PUBLIC 向下游传播
add_library(redstone-2d-core STATIC
    engine/Engine.cpp
    engine/Engine.h
    meta_component/Component.cpp
    meta_component/Component.h
    meta_component/Direction.h
    meta_component/RedstoneSignal.h
    model/GridModel.cpp
    model/GridModel.h
    world/World.cpp
    world/World.h
)
target_include_directories(redstone-2d-core PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(redstone-2d-core PUBLIC Qt::Core Qt::Gui)
