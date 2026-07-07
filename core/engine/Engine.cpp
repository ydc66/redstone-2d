#include "Engine.h"

#include <queue>

#include "core/world/World.h"
#include "core/model/GridModel.h"
#include "core/meta_component/Component.h"
#include "core/meta_component/Direction.h"

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::processTick(World *world)
{
    GridModel *grid = world->grid();
    if (!grid) return;

    std::queue<Component*> bfsQueue;

    // ═══════════════════════════════════════════════════════════
    //  Phase 1 — 信号源初始化 & 传输线清零
    // ═══════════════════════════════════════════════════════════
    for (int x = 0; x < grid->width(); ++x) {
        for (int y = 0; y < grid->height(); ++y) {
            auto *comp = grid->cellAt(x, y);
            if (!comp) continue;

            if (comp->isSignalSource()) {
                // 信号源赋初值
                comp->setOutputStrength(comp->basePowerLevel());
            }

            // 所有参与信号传播的元件加入 BFS 队列
            if (comp->isSignalSource() || comp->isTransceiver()) {
                bfsQueue.push(comp);
            } else {
                // 非参与元件（消费品、装饰等）清零输出，避免残留
                comp->setOutputStrength(0);
            }
        }
    }

    // ═══════════════════════════════════════════════════════════
    //  Phase 2 — BFS 信号传播
    // ═══════════════════════════════════════════════════════════
    while (!bfsQueue.empty()) {
        auto *comp = bfsQueue.front();
        bfsQueue.pop();

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
                    bfsQueue.push(neighbor);
            }
        }
    }

    // ═══════════════════════════════════════════════════════════
    //  Phase 2.5 — 构建信号快照（全网格信号物化）
    // ═══════════════════════════════════════════════════════════
    buildSignalCache(grid);

    // ═══════════════════════════════════════════════════════════
    //  Phase 3 — 消费者响应（引擎驱动，传入预计算信号）
    // ═══════════════════════════════════════════════════════════
    for (int x = 0; x < grid->width(); ++x) {
        for (int y = 0; y < grid->height(); ++y) {
            auto *comp = grid->cellAt(x, y);
            if (comp && comp->isConsumer())
                comp->onTick(m_signalCache[x][y]);
        }
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
