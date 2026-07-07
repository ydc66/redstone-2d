#include "Component.h"

#include <QPainter>
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
    : m_x(x), m_y(y)
    , m_facing(facing)
    , m_inputPorts(std::move(inputPorts))
    , m_outputPorts(std::move(outputPorts))
{
}

bool Component::canInputFrom(Direction absDir) const
{
    RelDir rel = toRelativeDir(absDir, m_facing);
    return m_inputPorts.contains(rel);
}

bool Component::canOutputTo(Direction absDir) const
{
    RelDir rel = toRelativeDir(absDir, m_facing);
    return m_outputPorts.contains(rel);
}

// ═══════════════════════════════════════════════════════════
//  渲染（Template Method）
// ═══════════════════════════════════════════════════════════

int Component::directionToAngle(Direction d) noexcept
{
    // North=0 → 0°, East=1 → 90°, South=2 → 180°, West=3 → 270°
    return static_cast<int>(d) * 90;
}

void Component::paint(QPainter *painter, int cellSize) const
{
    painter->save();
    // 绕格子中心旋转，使朝北的绘制结果适配实际朝向
    painter->translate(cellSize / 2.0, cellSize / 2.0);
    painter->rotate(directionToAngle(m_facing));
    painter->translate(-cellSize / 2.0, -cellSize / 2.0);
    paintContent(painter, cellSize);
    painter->restore();
}
