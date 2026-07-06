/**
 * @file   GridGraphicsScene.cpp
 * @brief  网格背景场景实现
 * @date   2026-07-05
 */

#include "GridGraphicsScene.h"

#include "core/GridModel.h"
#include "core/Component.h"

#include <QPainter>
#include <QColor>
#include <cmath>

GridGraphicsScene::GridGraphicsScene(GridModel *grid, QObject *parent)
    : QGraphicsScene(parent)
    , m_gridModel(grid)
{
    // 设置背景颜色
    setBackgroundBrush(QColor(QStringLiteral("#1e1e1e")));

    // 设置网格尺寸
    if (m_gridModel)
        setGridSize(m_gridModel->width(), m_gridModel->height(), m_cellSize);
    else
        updateSceneRect();
}

void GridGraphicsScene::setGridSize(int width, int height, int cellSize)
{
    m_gridWidth  = width;
    m_gridHeight = height;
    m_cellSize   = cellSize;
    updateSceneRect();
    invalidate();
}

void GridGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    // 绘制背景
    QGraphicsScene::drawBackground(painter, rect);

    // 计算网格尺寸
    const qreal gridW = m_gridWidth  * m_cellSize;
    const qreal gridH = m_gridHeight * m_cellSize;

    // 如果网格尺寸为 0，则不绘制网格
    if (gridW <= 0 || gridH <= 0)
        return;

    // 只绘制可见范围内的网格线
    const qreal left   = qMax<qreal>(0.0, rect.left());
    const qreal top    = qMax<qreal>(0.0, rect.top());
    const qreal right  = qMin<qreal>(gridW, rect.right());
    const qreal bottom = qMin<qreal>(gridH, rect.bottom());
    
    QPen gridPen(QColor(QStringLiteral("#3a3a3a")));
    painter->setPen(gridPen);

    // 竖线
    qreal x = std::floor(left / m_cellSize) * m_cellSize;
    while (x <= right) {
        painter->drawLine(QPointF(x, top), QPointF(x, bottom));
        x += m_cellSize;
    }

    // 横线
    qreal y = std::floor(top / m_cellSize) * m_cellSize;
    while (y <= bottom) {
        painter->drawLine(QPointF(left, y), QPointF(right, y));
        y += m_cellSize;
    }

    // 外边框
    QPen borderPen(QColor(QStringLiteral("#555555")));
    painter->setPen(borderPen);
    painter->drawRect(QRectF(0.0, 0.0, gridW, gridH));

    // 绘制元件
    drawComponents(painter, rect);
}

void GridGraphicsScene::drawComponents(QPainter *painter, const QRectF &rect)
{
    if (!m_gridModel)
        return;

    const int cs = m_cellSize;

    // 计算可见范围
    const int startX = qMax(0, static_cast<int>(rect.left()) / cs);
    const int startY = qMax(0, static_cast<int>(rect.top()) / cs);
    const int endX   = qMin(m_gridWidth - 1,
                            static_cast<int>(rect.right()) / cs);
    const int endY   = qMin(m_gridHeight - 1,
                            static_cast<int>(rect.bottom()) / cs);

    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            auto *comp = m_gridModel->cellAt(x, y);
            if (!comp || comp->isDefault())
                continue;  // 跳过 Air

            painter->save();
            painter->translate(x * cs, y * cs);
            comp->paint(painter, cs);
            painter->restore();
        }
    }
}

void GridGraphicsScene::updateSceneRect()
{
    const qreal w = m_gridWidth  * m_cellSize;
    const qreal h = m_gridHeight * m_cellSize;
    setSceneRect(-w * 0.1, -h * 0.1, w * 1.2, h * 1.2);
}
