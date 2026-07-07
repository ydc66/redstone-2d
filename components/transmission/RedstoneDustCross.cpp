#include "RedstoneDustCross.h"

#include <QPainter>

RedstoneDustCross::RedstoneDustCross(int x, int y)
    : RedstoneDust(x, y, Direction::North,
                   {RelDir::Front, RelDir::Right,
                    RelDir::Back,  RelDir::Left},
                   {RelDir::Front, RelDir::Right,
                    RelDir::Back,  RelDir::Left})
{
}

void RedstoneDustCross::paintContent(QPainter *painter, int cellSize) const
{
    const int p = outputStrength();       // 0..15
    const int r = 40 + p * 12;           // 40..220
    QColor color(r, 10, 10, 200);

    const int cx = cellSize / 2;
    const int cy = cellSize / 2;
    const int half = cellSize / 2 - 2;

    // 十字连线
    painter->setPen(QPen(color, cellSize / 6, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(cx - half, cy, cx + half, cy);   // 水平
    painter->drawLine(cx, cy - half, cx, cy + half);   // 垂直

    // 中心圆点
    painter->setBrush(color);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(cx, cy), cellSize / 5, cellSize / 5);
}
