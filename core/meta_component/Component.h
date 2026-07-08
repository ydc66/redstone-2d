#pragma once

#include <array>

#include "Direction.h"
#include "core/meta_component/RedstoneSignal.h"

#include <QList>
#include <QString>

class QPainter;
class GridModel;

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
    virtual void onInteract() {}
    /// @param sigArray 预计算的 4 方向信号（引擎 Phase 3 传入）
    virtual void onTick(const std::array<RedstoneSignal, 4>& sigArray) { (void)sigArray; }

    // ─── 信号系统（仿真引擎用） ───
    virtual bool isSignalSource()  const { return false; }
    virtual bool isTransceiver()   const { return false; }
    virtual bool isConsumer()      const { return false; }

    /// 是否标记为待销毁（tick 结束后由 World 统一清理）
    virtual bool isMarkedForRemoval() const { return false; }

    /// 输出是否为强充能（弱充能不可激活传输元件）
    virtual bool isStrongOutput()  const { return false; }

    int  outputStrength()    const { return m_outputStrength; }
    void setOutputStrength(int v)  { m_outputStrength = v; }

    /// Phase 2 BFS：根据邻居信号重新计算自身输出
    virtual void computeOutput(GridModel *grid);

    // ─── 渲染（Template Method：基类处理朝向旋转，子类只画朝北版本） ───
    void paint(QPainter *painter, int cellSize) const;

    // ─── 注册 ID（关联 ComponentRegistry 中的 entry） ───
    const QString& registryId() const { return m_registryId; }
    void setRegistryId(const QString &id) { m_registryId = id; }

protected:
    /// 子类实现此方法，按朝北方向绘制元件。基类 paint() 会根据 m_facing 旋转画布后再调用此方法。
    virtual void paintContent(QPainter *painter, int cellSize) const = 0;

    /// 朝向 → 旋转角度（顺时针），用于 paint() 中的画布旋转
    static int directionToAngle(Direction d) noexcept;

private:
    int             m_x, m_y;
    QString         m_registryId;
    Direction       m_facing        = Direction::North;

    QList<RelDir>   m_inputPorts;
    QList<RelDir>   m_outputPorts;

    int             m_outputStrength  = 0;
};
