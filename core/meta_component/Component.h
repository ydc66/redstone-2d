#pragma once

#include "Direction.h"

#include <QList>
#include <QString>

class QPainter;

/// 方块类型，决定了物理交互行为
enum class Category
{
    Air,        ///< 空气，不参与任何逻辑
    Solid,      ///< 实心方块，可被强/弱充能，阻挡信号穿过
    NonSolid    ///< 非固体（红石线、火把等），信号可穿过
};

/**
 * @brief 元件抽象基类
 *
 * 集数据与策略于一体，是所有方块/元件的公共基类。
 * 子类必须实现 paint() 和 category()，可按需重写 onInteract() / onTick() / isPushable() / basePowerLevel()。
 */
class Component
{
public:
    Component(int x, int y,
              Direction facing = Direction::North,
              QList<RelDir> inputPorts = {},
              QList<RelDir> outputPorts = {});

    virtual ~Component() = default;

    // ─── 位置 / 朝向 ───
    int x()       const { return m_x; }
    int y()       const { return m_y; }
    void setPosition(int x, int y) { m_x = x; m_y = y; }

    Direction facing()      const { return m_facing; }
    void setFacing(Direction d)    { m_facing = d; }

    // ─── 物理属性（子类自声明） ───
    virtual Category category()         const = 0;
    virtual bool     isPushable()       const { return false; }
    virtual int      basePowerLevel()   const { return 0; }

    bool isSolid() const { return category() == Category::Solid; }

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
    bool            m_dirty         = false;

    QList<RelDir>   m_inputPorts;
    QList<RelDir>   m_outputPorts;
};
