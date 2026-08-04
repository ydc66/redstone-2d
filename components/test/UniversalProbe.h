#pragma once

#include "core/meta_component/Component.h"

#include <array>
#include <QtGlobal>

/**
 * @brief 万能测试探针 — 元件单元测试的测试替身（test double）
 *
 * 按角色扮演信号源和/或信号接收器：
 *  - Source：可配置输出强度（0-15）与强弱充能，替代真实信号源
 *    （拉杆/红石块固定 0/15 且都是强充能，无法覆盖"强度 7 的弱充能"等场景）
 *  - Receiver：按方向记录自身收到的完整信号，替代真实消费者
 *    （灯/火把有延迟状态机，中间态不可读，无法断言精确强度）
 *  - Both：同时担任源与接收，组合测试的中间节点可放探针同时观测与供能
 *
 * 接口按领域语义设计（准确信号源 / 按方向的完整信号），不暴露引擎虚函数细节——
 * 方向由接口参数表达，不依赖 RedstoneSignal.direction 字段（计划删除的死字段），
 * 未来"信号与充能内聚到元件内部"的重构只影响内部旧架构适配层（虚函数覆盖），
 * 测试代码无需改动。
 *
 * 用法（测试内直接构造 + 放入网格，不经过 Registry）：
 *   auto src = std::make_unique<UniversalProbe>(1, 1, UniversalProbe::Role::Source);
 *   src->setSourceStrength(15);
 *   grid.placeComponent(1, 1, std::move(src));
 */
class UniversalProbe : public Component
{
public:
    enum class Role
    {
        Source,    ///< 仅信号源（isSignalSource）
        Receiver,  ///< 仅接收器（isConsumer）
        Both       ///< 同时担任（既是源又是接收器）
    };

    UniversalProbe(int x, int y, Role role = Role::Receiver)
        : Component(x, y, Direction::North,
                    {RelDir::Front, RelDir::Right, RelDir::Back, RelDir::Left},  // 全向输入
                    {RelDir::Front, RelDir::Right, RelDir::Back, RelDir::Left})  // 全向输出
        , m_role(role)
    {
    }

    // ─── 源侧配置（Source / Both 角色生效；tick 间可改，Engine Phase 1 每次读取） ───
    void setSourceStrength(int strength)     { m_sourceStrength = strength; }
    void setStrongOutputEnabled(bool strong) { m_strong = strong; }
    int  configuredStrength() const          { return m_sourceStrength; }

    // ─── 接收侧观测（Receiver / Both 角色生效） ───
    /// 按方向查询收到的信号（方向 = 信号来源方向）；未收到过信号时返回空信号
    RedstoneSignal receivedSignal(Direction dir) const
    {
        if (dir == Direction::None)
            return {};
        return m_received[static_cast<int>(dir)];
    }

    // ─── 旧架构适配层（未来信号内聚重构后重写这里即可） ───
    Category category() const override { return Category::NonSolid; }

    bool isSignalSource() const override { return m_role != Role::Receiver; }
    bool isConsumer()     const override { return m_role != Role::Source; }
    bool isStrongOutput() const override { return m_strong; }
    int  basePowerLevel() const override { return m_sourceStrength; }

    void computeOutput(GridModel *) override
    {
        // Phase 2 保持配置强度：源探针输出不依赖邻居，防被 BFS 按邻居信号改写
        setOutputStrength(m_sourceStrength);
    }

    void onTick(const std::array<RedstoneSignal, 4> &sigArray) override
    {
        // 全向接收：逐方向记录完整信号（索引 = static_cast<int>(Direction)，与引擎缓存一致）
        m_received = sigArray;
    }

protected:
    void paintContent(QPainter *, int) const override {}    // 探针不渲染

private:
    Role                  m_role;
    int                   m_sourceStrength = 0;
    bool                  m_strong         = false;
    std::array<RedstoneSignal, 4> m_received;   // 按方向最近收到的信号（默认空信号）
};
