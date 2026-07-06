/**
 * @file   Interaction.cpp
 * @brief  交互模式策略基类实现
 * @date   2026-07-05
 */

#include "Interaction.h"
#include "../GridGraphicsScene.h"

#include <QGraphicsView>
#include <QKeyEvent>

Interaction::Interaction(GridModel *grid, GridGraphicsScene *scene,
                         QGraphicsView *view, QObject *parent)
    : QObject(parent)
    , m_grid(grid)
    , m_scene(scene)
    , m_view(view)
{
}

// 默认空实现
void Interaction::onLeftPress(const QPointF &) {}
void Interaction::onLeftMove(const QPointF &) {}
void Interaction::onLeftRelease(const QPointF &) {}
void Interaction::onKeyPress(QKeyEvent *) {}
void Interaction::onActivate() {}
void Interaction::onDeactivate() {}

QCursor Interaction::cursor() const
{
    return Qt::ArrowCursor;
}

QPoint Interaction::scenePosToGrid(const QPointF &scenePos) const
{
    const int cs = m_scene ? m_scene->cellSize() : 64;
    return QPoint(static_cast<int>(scenePos.x()) / cs,
                  static_cast<int>(scenePos.y()) / cs);
}
