/**
 * @file   Interaction.cpp
 * @brief  交互模式策略基类实现
 * @date   2026-07-05
 */

#include "Interaction.h"
#include "../GridGraphicsScene.h"

#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>

Interaction::Interaction(World *world, GridGraphicsScene *scene,
                         QGraphicsView *view, QObject *parent)
    : QObject(parent)
    , m_world(world)
    , m_scene(scene)
    , m_view(view)
{
    // 基类仅存储指针，子类在各自的事件处理中使用
}

// ─── 默认空实现（子类按需覆写） ───
void Interaction::onLeftPress(const QPointF &) {}
void Interaction::onLeftMove(const QPointF &) {}
void Interaction::onLeftRelease(const QPointF &) {}
void Interaction::onRightPress(const QPointF &) {}
void Interaction::onRightRelease(const QPointF &) {}
void Interaction::onRightMove(const QPointF &) {}
void Interaction::onMouseMove(const QPointF &) {}
void Interaction::onKeyPress(QKeyEvent *) {}
void Interaction::onWheel(QWheelEvent *) {}
void Interaction::onActivate() {}
void Interaction::onDeactivate() {}

/** @brief 默认返回箭头光标，子类可按需覆写 */
QCursor Interaction::cursor() const
{
    return Qt::ArrowCursor;
}

/**
 * @brief   将场景坐标转换为网格坐标
 * @param   scenePos 场景坐标（浮点）
 * @return  网格坐标（行列），向下取整
 */
QPoint Interaction::scenePosToGrid(const QPointF &scenePos) const
{
    const int cs = m_scene ? m_scene->cellSize() : 64;
    return QPoint(static_cast<int>(scenePos.x()) / cs,
                  static_cast<int>(scenePos.y()) / cs);
}
