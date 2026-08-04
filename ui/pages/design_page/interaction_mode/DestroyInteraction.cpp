/**
 * @file   DestroyInteraction.cpp
 * @brief  破坏模式 — 点击移除元件
 */

#include "DestroyInteraction.h"

#include "core/world/World.h"
#include "core/meta_component/Component.h"

void DestroyInteraction::onLeftPress(const QPointF &scenePos)
{
    const QPoint grid = scenePosToGrid(scenePos);
    if (!m_world || !m_world->isValid(grid.x(), grid.y()))
        return;

    auto *comp = m_world->cellAt(grid.x(), grid.y());
    if (!comp)
        return;

    const QString id = QString::number(grid.x()) + QStringLiteral(",")
                       + QString::number(grid.y());
    emit componentDestroyed(id, grid.x(), grid.y());
}

QCursor DestroyInteraction::cursor() const
{
    return Qt::CrossCursor;
}
