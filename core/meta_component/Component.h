#pragma once

#include "Direction.h"
#include "ComponentSpec.h"

#include <QList>
#include <QString>

class QPainter;

/**
 * @brief 元件抽象基类
 *
 * 集数据与策略于一体，是所有方块/元件的公共基类。
 * 子类必须实现 paint()，可按需重写 onInteract() 和 onTick()。
 */
class Component
{
public:
    Component(int x, int y, Direction facing,
              Category category, bool isPushable, int basePowerLevel,
              QList<RelDir> inputPorts,
              QList<RelDir> outputPorts);

    virtual ~Component() = default;

    // ─── 位置 / 朝向 ───
    int x()       const { return m_x; }
    int y()       const { return m_y; }
    void setPosition(int x, int y) { m_x = x; m_y = y; }

    Direction facing()      const { return m_facing; }
    void setFacing(Direction d)    { m_facing = d; }

    // ─── 物理属性 ───
    Category category()       const { return m_category; }
    bool isSolid()            const { return m_category == Category::Solid; }
    bool isPushable()         const { return m_isPushable; }
    int  basePowerLevel()     const { return m_basePowerLevel; }

    // ─── 端口查询 ───
    const QList<RelDir>& inputPorts()  const { return m_inputPorts; }
    const QList<RelDir>& outputPorts() const { return m_outputPorts; }

    bool canInputFrom(Direction absDir)  const;
    bool canOutputTo(Direction absDir)   const;

    // ─── 虚方法：子类按需重写 ───
    virtual void paint(QPainter *painter, int cellSize) const = 0;
    virtual void onInteract() {}
    virtual void onTick() {}

    // ─── 脏标志（引擎调度用） ───
    bool isDirty()    const { return m_dirty; }
    void clearDirty()       { m_dirty = false; }
    void markDirty()        { m_dirty = true; }

    // ─── 注册 ID（关联 ComponentRegistry 中的 entry） ───
    const QString& registryId() const { return m_registryId; }
    void setRegistryId(const QString &id) { m_registryId = id; }

private:
    int             m_x, m_y;
    QString         m_registryId;
    Direction       m_facing        = Direction::North;
    Category        m_category      = Category::Air;
    bool            m_isPushable    = false;
    int             m_basePowerLevel = 0;
    bool            m_dirty         = false;

    QList<RelDir>   m_inputPorts;
    QList<RelDir>   m_outputPorts;
};
