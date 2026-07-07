/**
 * @file   PlaceInteraction.cpp
 * @brief  放置模式 — 点击网格放置元件
 */

#include "PlaceInteraction.h"

#include "core/model/GridModel.h"
#include "core/types/Component.h"

void PlaceInteraction::onLeftPress(const QPointF &scenePos)
{
    if (m_activeComponentId.isEmpty())
        return;

    const QPoint grid = scenePosToGrid(scenePos);
    if (!canPlaceAt(grid.x(), grid.y()))
        return;

    emit placeRequested(grid.x(), grid.y(), m_activeComponentId);
}

bool PlaceInteraction::canPlaceAt(int gx, int gy) const
{
    if (!m_grid || !m_grid->isValid(gx, gy))
        return false;
    auto *comp = m_grid->cellAt(gx, gy);
    return !comp;
}

void PlaceInteraction::setActiveComponent(const QString &componentId)
{
    m_activeComponentId = componentId;
}

QCursor PlaceInteraction::cursor() const
{
    if (m_activeComponentId.isEmpty())
        return Qt::ForbiddenCursor;
    return Qt::CrossCursor;
}
