#include "RedstoneTorchBase.h"

RedstoneTorchBase::RedstoneTorchBase(int x, int y,
                                      Direction facing,
                                      QList<RelDir> inputPorts,
                                      QList<RelDir> outputPorts,
                                      int tickDelay)
    : Component(x, y, facing, std::move(inputPorts), std::move(outputPorts))
    , m_tickDelay(tickDelay)
{
}

void RedstoneTorchBase::computeOutput(GridModel *)
{
    // ─── 延迟状态切换 ───
    if (m_lit != m_pendingLit) {
        // 防 BFS 重入：同一 tick 内多次 computeOutput 只计一次
        if (!m_transitionCounted) {
            ++m_transitionTicks;
            m_transitionCounted = true;
        }
        if (m_transitionTicks >= m_tickDelay) {
            m_lit = m_pendingLit;
            m_transitionTicks = 0;
        }
    } else {
        m_transitionTicks = 0;
    }

    setOutputStrength(m_lit ? 15 : 0);
}

void RedstoneTorchBase::onTick(const std::array<RedstoneSignal, 4>& sigArray)
{
    // 下一 tick 的 BFS 重新允许计数
    m_transitionCounted = false;

    // ─── 有且只有背面被充能时才熄灭 ───
    bool hasSignal = false;
    for (Direction dir : allDirections()) {
        if (!canInputFrom(dir))
            continue;
        if (sigArray[static_cast<int>(dir)].strength > 0) {
            hasSignal = true;
            break;
        }
    }
    m_pendingLit = !hasSignal;
}
