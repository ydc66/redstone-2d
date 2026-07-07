#pragma once

#include <memory>

class GridModel;
class Engine;

/**
 * @brief 世界 — 核心聚合根
 *
 * 持有 GridModel 和仿真引擎，对外提供仿真控制接口。
 * 所有对网格和仿真的访问都通过 World 进行。
 */
class World
{
public:
    World();
    ~World();

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    /// 获取网格模型
    GridModel* grid() const { return m_grid.get(); }

    // ─── 仿真控制 ───
    /// 启动仿真循环
    void start();
    /// 停止仿真循环
    void stop();
    /// 单步执行一个 tick
    void singleTick();

    /// 是否正在仿真
    bool isRunning() const;
    /// 已执行 tick 总数
    int tickCount() const { return m_tick; }

private:
    std::unique_ptr<GridModel> m_grid;
    std::unique_ptr<Engine>    m_engine;

    int  m_tick    = 0;
    bool m_running = false;
};
