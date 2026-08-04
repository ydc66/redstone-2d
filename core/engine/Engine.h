#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "core/meta_component/RedstoneSignal.h"

class GridModel;
class Component;

/**
 * @brief 仿真引擎
 *
 * 管理红石电路的 tick 模拟。
 * Phase 1~3 信号传播 + 内部维护信号快照缓存。
 * 只依赖 GridModel（只读），不感知 World/UI。
 */
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    /// 执行一个 tick（只读 grid，不修改网格结构）
    void processTick(GridModel *grid);

private:
    /// 信号快照：cache[x][y][dirIndex] 对应 North=0, East=1, South=2, West=3
    using SignalCache = std::vector<std::vector<std::array<RedstoneSignal, 4>>>;
    SignalCache m_signalCache;

    /// BFS 队列缓冲（成员复用：clear 保留 capacity，热路径零分配）
    std::vector<Component *> m_bfsQueue;
    /// 队列头索引（已处理元素数量），避免 pop 移动数据
    std::size_t m_bfsHead = 0;

    /// 遍历全网格，将当前所有方向的输入信号快照到 m_signalCache
    void buildSignalCache(GridModel *grid);
};
