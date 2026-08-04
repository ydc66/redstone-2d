#pragma once

#include <array>

#include "core/meta_component/WorldObject.h"
#include "core/meta_component/RedstoneSignal.h"

#include <QList>
#include <QString>

class GridModel;

/// 方块类型，决定了物理交互行为
enum class Category
{
    Air,        ///< 空气，不参与任何逻辑
    Solid,      ///< 实心方块，可被强/弱充能，阻挡信号穿过
    NonSolid    ///< 非固体（红石线、火把等），信号可穿过
};

/**
 * @brief 红石元件抽象基类
 *
 * 继承 WorldObject（网格占位 + 朝向 + 渲染），在此之上提供红石逻辑：
 * 物理属性、端口系统、信号系统、交互与注册 ID。
 * 子类必须实现 paintContent() 和 category()，可按需重写 onInteract() / onTick() / isPushable() / basePowerLevel()。
 */
class Component : public WorldObject
{
public:
    Component(int x, int y,
              Direction facing = Direction::North,
              QList<RelDir> inputPorts = {},
              QList<RelDir> outputPorts = {});

    virtual ~Component() = default;

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

    // ─── 注册 ID（关联 ComponentRegistry 中的 entry） ───
    const QString& registryId() const { return m_registryId; }
    void setRegistryId(const QString &id) { m_registryId = id; }

private:
    QString         m_registryId;

    QList<RelDir>   m_inputPorts;
    QList<RelDir>   m_outputPorts;

    int             m_outputStrength  = 0;
};
