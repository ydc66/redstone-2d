/**
 * @file   DestroyInteraction.cpp
 * @brief  破坏模式 — 点击移除元件
 */

#include "DestroyInteraction.h"

#include "core/model/GridModel.h"
#include "core/types/Component.h"

void DestroyInteraction::onLeftPress(const QPointF &scenePos)
{
    const QPoint grid = scenePosToGrid(scenePos);
    if (!m_grid || !m_grid->isValid(grid.x(), grid.y()))
        return;

    auto *comp = m_grid->cellAt(grid.x(), grid.y());
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
