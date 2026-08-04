#include "Engine.h"

#include "core/model/GridModel.h"
#include "core/meta_component/Component.h"
#include "core/meta_component/Direction.h"

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::processTick(GridModel *grid)
{
    if (!grid) return;

    // 复用 BFS 队列缓冲（clear 保留 capacity，避免每 tick 堆分配）
    m_bfsQueue.clear();
    m_bfsHead = 0;

    // ═══════════════════════════════════════════════════════════
    //  Phase 1 — 信号源初始化 & 传输线清零
    //  （稀疏遍历：只走活动元件列表，不扫描空气格）
    // ═══════════════════════════════════════════════════════════
    for (auto *comp : grid->activeComponents()) {
        if (comp->isSignalSource()) {
            // 信号源赋初值
            comp->setOutputStrength(comp->basePowerLevel());
        }

        // 所有参与信号传播的元件加入 BFS 队列
        if (comp->isSignalSource() || comp->isTransceiver()) {
            m_bfsQueue.push_back(comp);
        } else {
            // 非参与元件（消费品、装饰等）清零输出，避免残留
            comp->setOutputStrength(0);
        }
    }

    // ═══════════════════════════════════════════════════════════
    //  Phase 2 — BFS 信号传播
    // ═══════════════════════════════════════════════════════════
    while (m_bfsHead < m_bfsQueue.size()) {
        auto *comp = m_bfsQueue[m_bfsHead++];

        int oldOut = comp->outputStrength();
        comp->computeOutput(grid);
        int newOut = comp->outputStrength();

        if (newOut != oldOut) {
            // 输出变化 → 通知所有可传播的邻居
            for (Direction dir : {Direction::North, Direction::East,
                                  Direction::South, Direction::West}) {
                int nx = comp->x() + dx(dir);
                int ny = comp->y() + dy(dir);
                if (!grid->isValid(nx, ny)) continue;

                auto *neighbor = grid->cellAt(nx, ny);
                if (neighbor && neighbor->isTransceiver())
                    m_bfsQueue.push_back(neighbor);
            }
        }
    }

    // ═══════════════════════════════════════════════════════════
    //  Phase 2.5 — 构建信号快照（全网格信号物化）
    // ═══════════════════════════════════════════════════════════
    buildSignalCache(grid);

    // ═══════════════════════════════════════════════════════════
    //  Phase 3 — 消费者响应（引擎驱动，传入预计算信号）
    //  （稀疏遍历：只走活动元件列表）
    // ═══════════════════════════════════════════════════════════
    for (auto *comp : grid->activeComponents()) {
        if (comp->isConsumer())
            comp->onTick(m_signalCache[comp->x()][comp->y()]);
    }
}

void Engine::buildSignalCache(GridModel *grid)
{
    m_signalCache.resize(grid->width());
    for (int x = 0; x < grid->width(); ++x) {
        m_signalCache[x].resize(grid->height());
        for (int y = 0; y < grid->height(); ++y) {
            auto& cellCache = m_signalCache[x][y];
            for (Direction dir : allDirections()) {
                int idx = static_cast<int>(dir);   // North=0, East=1, South=2, West=3
                cellCache[idx] = grid->signalFrom(x, y, dir);
            }
        }
    }
}
