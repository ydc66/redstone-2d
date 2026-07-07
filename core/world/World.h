#pragma once

#include <memory>

#include <QObject>

class QTimer;

#include "core/meta_component/Direction.h"

class GridModel;
class Engine;
class Component;

/**
 * @brief 世界 — 核心聚合根
 *
 * 持有 GridModel 和仿真引擎，对外提供仿真控制接口。
 * 所有对网格和仿真的访问都通过 World 进行。
 */
class World : public QObject
{
    Q_OBJECT

public:
    explicit World(QObject *parent = nullptr);
    ~World() override;

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    /// 获取网格模型
    GridModel* grid() const { return m_grid.get(); }

    /// 查询 (x,y) 从 dir 方向传入的信号强度
    int querySignal(int x, int y, Direction dir) const;

    // ─── 仿真控制 ───
    /// 启动仿真循环
    void start();
    /// 停止仿真循环
    void stop();
    /// 单步执行一个 tick
    void singleTick();
    /// 设置仿真速度倍率
    void setSpeed(double speed);

    /// 是否正在仿真
    bool isRunning() const { return m_running; }
    /// 已执行 tick 总数
    int tickCount() const { return m_tick; }

signals:
    /// 每次 tick 完成后发射
    void tickCompleted(int tickCount);

private slots:
    /// 定时器触发 → 执行一个 tick
    void onTimerTick();

private:
    std::unique_ptr<GridModel> m_grid;
    std::unique_ptr<Engine>    m_engine;
    QTimer                    *m_timer   = nullptr;

    int    m_tick    = 0;
    bool   m_running = false;
    double m_speed   = 1.0;
};
