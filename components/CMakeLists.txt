# ─── 元件库：具体红石元件 + 注册表（依赖 core，测试与主程序共用）───
add_library(redstone-2d-components STATIC
    blocks/DirectionalBlock.cpp
    blocks/DirectionalBlock.h
    blocks/SolidBlock.cpp
    blocks/SolidBlock.h
    mechanical/RedstoneLamp.cpp
    mechanical/RedstoneLamp.h
    registration/ComponentRegistrator.cpp
    registration/ComponentRegistrator.h
    registration/ComponentRegistry.cpp
    registration/ComponentRegistry.h
    sources/Lever.cpp
    sources/Lever.h
    sources/RedstoneBlock.cpp
    sources/RedstoneBlock.h
    transmission/RedstoneDust.cpp
    transmission/RedstoneDust.h
    transmission/RedstoneDustCross.cpp
    transmission/RedstoneDustCross.h
    transmission/RedstoneDustLine.cpp
    transmission/RedstoneDustLine.h
    transmission/RedstoneTorchBase.cpp
    transmission/RedstoneTorchBase.h
    transmission/RedstoneTorchGround.cpp
    transmission/RedstoneTorchGround.h
    transmission/RedstoneTorchWall.cpp
    transmission/RedstoneTorchWall.h
)
target_link_libraries(redstone-2d-components PUBLIC redstone-2d-core)
