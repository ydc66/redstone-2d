#include "Component.h"

#include <algorithm>

// ═══════════════════════════════════════════════════════════
//  默认信号实现（无输入输出的普通元件直接忽略）
// ═══════════════════════════════════════════════════════════

void Component::computeOutput(GridModel *)
{
    // 默认：不改变输出
}

Component::Component(int x, int y,
                     Direction facing,
                     QList<RelDir> inputPorts,
                     QList<RelDir> outputPorts)
    : WorldObject(x, y, facing)
    , m_inputPorts(std::move(inputPorts))
    , m_outputPorts(std::move(outputPorts))
{
}

bool Component::canInputFrom(Direction absDir) const
{
    RelDir rel = toRelativeDir(absDir, facing());
    return m_inputPorts.contains(rel);
}

bool Component::canOutputTo(Direction absDir) const
{
    RelDir rel = toRelativeDir(absDir, facing());
    return m_outputPorts.contains(rel);
}
