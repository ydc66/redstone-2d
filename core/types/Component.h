#pragma once

#include "Behavior.h"
#include "Direction.h"
#include "ComponentSpec.h"

#include <QList>
#include <QString>
#include <memory>

class QPainter;

class Component
{
public:
    Component(int x, int y, Direction facing, ComponentSpec spec,
              QList<RelDir> inputPorts,
              QList<RelDir> outputPorts,
              std::unique_ptr<Behavior> behavior);

    virtual ~Component() = default;

    // ─── 位置 / 朝向 ───
    int x()       const { return m_x; }
    int y()       const { return m_y; }
    void setPosition(int x, int y) { m_x = x; m_y = y; }

    Direction facing()      const { return m_facing; }
    void setFacing(Direction d)    { m_facing = d; }

    // ─── 物理属性 ───
    const ComponentSpec& spec() const { return m_spec; }
    bool isSolid() const { return m_spec.category == Category::Solid; }

    // ─── 端口查询 ───
    const QList<RelDir>& inputPorts()  const { return m_inputPorts; }
    const QList<RelDir>& outputPorts() const { return m_outputPorts; }

    bool canInputFrom(Direction absDir)  const;
    bool canOutputTo(Direction absDir)   const;

    // ─── 渲染（委托 Behavior） ───
    void paint(QPainter *painter, int cellSize) const;

    // ─── 交互（委托 Behavior） ───
    void onInteract() { if (m_behavior) m_behavior->onInteract(); }

    // ─── 脏标志 ───
    bool isDirty()    const { return m_dirty; }
    void clearDirty()       { m_dirty = false; }
    void markDirty()        { m_dirty = true; }

    // ─── 注册 ID（关联 ComponentRegistry 中的 entry） ───
    const QString& registryId() const { return m_registryId; }
    void setRegistryId(const QString &id) { m_registryId = id; }

private:
    int             m_x, m_y;
    QString         m_registryId;
    Direction       m_facing    = Direction::North;
    ComponentSpec   m_spec;
    bool            m_dirty     = false;

    QList<RelDir>   m_inputPorts;
    QList<RelDir>   m_outputPorts;

    std::unique_ptr<Behavior> m_behavior;
};
