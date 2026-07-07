#include "RedstoneDustLine.h"

#include <QPainter>

RedstoneDustLine::RedstoneDustLine(int x, int y)
    : RedstoneDust(x, y, Direction::North,
                   {RelDir::Front, RelDir::Back},
                   {RelDir::Front, RelDir::Back})
{
}

void RedstoneDustLine::paintContent(QPainter *painter, int cellSize) const
{
    const int p = outputStrength();       // 0..15
    const int r = 40 + p * 12;           // 40..220
    QColor color(r, 10, 10, 200);

    const int cx = cellSize / 2;
    const int cy = cellSize / 2;
    const int half = cellSize / 2 - 2;

    // 朝北画竖线，基类 paint() 根据 facing 旋转适配方向
    painter->setPen(QPen(color, cellSize / 6, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(cx, cy - half, cx, cy + half);

    // 中心圆点
    painter->setBrush(color);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(cx, cy), cellSize / 5, cellSize / 5);
}

void RedstoneDustLine::onInteract()
{
    setFacing(rotateRight(facing()));
}
