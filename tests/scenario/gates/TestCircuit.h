#pragma once

// ═══════════════════════════════════════════════════════════════
//  场景测试公共辅助（header-only，不参与构建产物）
//
//  只依赖 GridModel + Engine（不触碰 World / UI）：
//    - placeXxx()：便捷放置元件（封装 unique_ptr 构造）
//    - at<T>()：类型安全取元件指针（dynamic_cast）
//    - runTicks()：连续运行 n 个 tick（火把状态切换延迟 1 tick，
//      默认 3 tick 覆盖"火把切换 + 传播 + 消费者响应"整链）
// ═══════════════════════════════════════════════════════════════

#include <memory>

#include "core/model/GridModel.h"
#include "core/engine/Engine.h"
#include "core/meta_component/Component.h"

#include "components/blocks/SolidBlock.h"
#include "components/mechanical/RedstoneLamp.h"
#include "components/sources/Lever.h"
#include "components/transmission/RedstoneDustCross.h"
#include "components/transmission/RedstoneDustLine.h"
#include "components/transmission/RedstoneTorchWall.h"

namespace test_circuit {

// ─── 放置辅助 ───

inline void placeLever(GridModel &g, int x, int y)
{
    g.placeComponent(x, y, std::make_unique<Lever>(x, y));
}

inline void placeSolidBlock(GridModel &g, int x, int y)
{
    g.placeComponent(x, y, std::make_unique<SolidBlock>(x, y));
}

inline void placeTorch(GridModel &g, int x, int y)
{
    g.placeComponent(x, y, std::make_unique<RedstoneTorchWall>(x, y));
}

inline void placeLine(GridModel &g, int x, int y)
{
    g.placeComponent(x, y, std::make_unique<RedstoneDustLine>(x, y));
}

// 一字粉默认 facing=North（竖直），水平放置需显式旋转
inline void placeLineEast(GridModel &g, int x, int y)
{
    g.placeComponent(x, y, std::make_unique<RedstoneDustLine>(x, y));
    g.cellAt(x, y)->setFacing(Direction::East);
}

inline void placeLineWest(GridModel &g, int x, int y)
{
    g.placeComponent(x, y, std::make_unique<RedstoneDustLine>(x, y));
    g.cellAt(x, y)->setFacing(Direction::West);
}

inline void placeCross(GridModel &g, int x, int y)
{
    g.placeComponent(x, y, std::make_unique<RedstoneDustCross>(x, y));
}

inline void placeLamp(GridModel &g, int x, int y)
{
    g.placeComponent(x, y, std::make_unique<RedstoneLamp>(x, y));
}

// ─── 查询辅助 ───

template <typename T>
T *at(GridModel &g, int x, int y)
{
    return dynamic_cast<T *>(g.cellAt(x, y));
}

// ─── 运行辅助 ───

inline void runTicks(Engine &engine, GridModel &grid, int n = 3)
{
    for (int i = 0; i < n; ++i)
        engine.processTick(&grid);
}

} // namespace test_circuit
