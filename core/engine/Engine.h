#pragma once

#include <array>
#include <vector>

#include "core/meta_component/RedstoneSignal.h"

class GridModel;

/**
 * @brief 仿真引擎
 *
 * 管理红石电路的 tick 模拟。
 * Phase 1~3 信号传播 + 内部维护信号快照缓存。
 */
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    /// 执行一个 tick
    void processTick(class World *world);

private:
    /// 信号快照：cache[x][y][dirIndex] 对应 North=0, East=1, South=2, West=3
    using SignalCache = std::vector<std::vector<std::array<RedstoneSignal, 4>>>;
    SignalCache m_signalCache;

    /// 遍历全网格，将当前所有方向的输入信号快照到 m_signalCache
    void buildSignalCache(GridModel *grid);
};
