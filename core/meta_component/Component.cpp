#include "Component.h"

#include <algorithm>

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
