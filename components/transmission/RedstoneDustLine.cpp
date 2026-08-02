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
    const int r = 135 + p * 8;           // 135..255（EGE 16 级色阶 0x87..0xff）
    QColor color(r, 0, 0);

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
