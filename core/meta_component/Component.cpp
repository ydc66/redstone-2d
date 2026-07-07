#include "Component.h"

#include <QPainter>
#include <algorithm>

Component::Component(int x, int y, Direction facing, ComponentSpec spec,
                     QList<RelDir> inputPorts,
                     QList<RelDir> outputPorts,
                     std::unique_ptr<Behavior> behavior)
    : m_x(x), m_y(y)
    , m_facing(facing)
    , m_spec(spec)
    , m_inputPorts(std::move(inputPorts))
    , m_outputPorts(std::move(outputPorts))
    , m_behavior(std::move(behavior))
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

void Component::paint(QPainter *painter, int cellSize) const
{
    m_behavior->paint(painter, cellSize);
}
