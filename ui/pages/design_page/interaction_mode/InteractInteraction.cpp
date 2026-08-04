#include "InteractInteraction.h"

#include "core/world/World.h"
#include "core/meta_component/Component.h"

void InteractInteraction::onLeftPress(const QPointF &scenePos)
{
    const QPoint grid = scenePosToGrid(scenePos);
    if (!m_world || !m_world->isValid(grid.x(), grid.y()))
        return;

    auto *comp = m_world->cellAt(grid.x(), grid.y());
    if (!comp)
        return;

    emit interactTriggered(comp->registryId(), grid.x(), grid.y());
}

QCursor InteractInteraction::cursor() const
{
    return Qt::PointingHandCursor;
}
