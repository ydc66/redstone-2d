#include "Component.h"

#include <QPainter>
#include <algorithm>

Component::Component(int x, int y, Direction facing,
                     Category category, bool isPushable, int basePowerLevel,
                     QList<RelDir> inputPorts,
                     QList<RelDir> outputPorts)
    : m_x(x), m_y(y)
    , m_facing(facing)
    , m_category(category)
    , m_isPushable(isPushable)
    , m_basePowerLevel(basePowerLevel)
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
