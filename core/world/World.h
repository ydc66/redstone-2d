#pragma once

#include <memory>

#include <QObject>
#include <QString>

class QTimer;

#include "core/meta_component/Direction.h"

class GridModel;
class Engine;
class Component;

/**
 * @brief 世界 — 核心聚合根
 *
 * 持有 GridModel 和仿真引擎，对外提供仿真控制与编辑（放置/破坏/交互/移动）接口。
 * 所有对网格和仿真的访问都通过 World 进行；UI 层只依赖 World，不直接碰 GridModel。
 */
class World : public QObject
{
    Q_OBJECT

public:
    explicit World(QObject *parent = nullptr);
    ~World() override;

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    /// 获取网格模型（只读渲染等内部访问用，UI 层编辑请走编辑接口）
    GridModel* grid() const { return m_grid.get(); }

    // ─── 编辑操作（唯一编辑入口） ───
    /// 放置元件：校验（越界/占用/注册表创建失败）后入格，成功返回 true
    bool placeComponent(int x, int y, const QString &componentId,
                        Direction facing = Direction::North);
    /// 移除元件：返回被移除的指针（失败返回 nullptr）
    std::unique_ptr<Component> removeComponentAt(int x, int y);
    /// 移动元件：remove + place 原子收口，目标格非法/占用时回滚并返回 false
    bool moveComponent(int fromX, int fromY, int toX, int toY);
    /// 交互：委托给元件内部 onInteract()
    void interactAt(int x, int y);
    /// 只读查询代理
    Component* cellAt(int x, int y) const;
    bool isValid(int x, int y) const;
    /// 网格尺寸调整代理
    void resizeGrid(int w, int h);

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
    /// 元件放置成功后发射
    void componentPlaced(int x, int y);
    /// 元件移除成功后发射
    void componentRemoved(int x, int y);
    /// 元件交互（onInteract）后发射
    void componentInteracted(int x, int y);

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
