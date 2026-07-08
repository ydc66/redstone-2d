#pragma once

#include "core/meta_component/Component.h"

class QPainter;

/**
 * @brief 红石火把抽象基类
 *
 * 内置延迟状态机：
 *   - onTick() 读取输入信号 → 决定目标状态 m_pendingLit
 *   - computeOutput() 延迟切换 m_lit → m_pendingLit（延迟可配）
 *   输出统一为弱充能（isStrongOutput=false），强度 15。
 *
 * 子类只需在构造函数中设定端口列表，并实现 paintContent()。
 */
class RedstoneTorchBase : public Component
{
public:
    Category category()       const override { return Category::NonSolid; }
    bool     isTransceiver()  const override { return true; }
    bool     isConsumer()     const override { return true; }
    bool     isStrongOutput() const override { return false; }

    void computeOutput(GridModel *grid) override;
    void onTick(const std::array<RedstoneSignal, 4>& sigArray) override;

    /// 当前是否点亮
    bool isLit() const { return m_lit; }

    /// 状态切换延迟 tick 数（可动态调整，用于测试调试）
    int  tickDelay()       const { return m_tickDelay; }
    void setTickDelay(int d)     { m_tickDelay = qBound(0, d, 100); }

protected:
    explicit RedstoneTorchBase(int x, int y,
                                Direction facing,
                                QList<RelDir> inputPorts,
                                QList<RelDir> outputPorts,
                                int tickDelay = 1);

private:
    bool m_lit             = true;  ///< 当前点亮状态
    bool m_pendingLit      = true;  ///< 待决状态
    int  m_transitionTicks = 0;     ///< 当前过渡已持续的 tick 数
    bool m_transitionCounted = false; ///< 当前 BFS 中是否已计入 transitionTicks

    int  m_tickDelay  = 1;          ///< 状态切换延迟（默认 1 tick）
};
