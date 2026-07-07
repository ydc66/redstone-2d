#include "RedstoneDust.h"

#include "core/model/GridModel.h"
#include "core/meta_component/Direction.h"

#include <QPainter>
#include <algorithm>

RedstoneDust::RedstoneDust(int x, int y)
    : Component(x, y,
                Direction::North,
                {RelDir::Front, RelDir::Right,                   // 四方向输入
                 RelDir::Back,  RelDir::Left},
                {RelDir::Front, RelDir::Right,                   // 四方向输出
                 RelDir::Back,  RelDir::Left})
{
}

void RedstoneDust::computeOutput(GridModel *grid)
{
    int maxInput = 0;

    for (Direction dir : {Direction::North, Direction::East,
                          Direction::South, Direction::West}) {
        RedstoneSignal sig = grid->signalFrom(x(), y(), dir);

        // 只有强充能才能激活红石粉
        if (!sig.isStrong) continue;
        if (sig.strength > maxInput)
            maxInput = sig.strength;
    }
    setOutputStrength(std::max(0, maxInput - 1));
}

void RedstoneDust::paintContent(QPainter *painter, int cellSize) const
{
    const int p = outputStrength();       // 0..15
    const int r = 40 + p * 12;           // 40..220
    const int g = 10;
    const int b = 10;

    QColor color(r, g, b, 200);

    painter->setPen(QPen(color, cellSize / 6, Qt::SolidLine, Qt::RoundCap));

    // 十字连线（简化：四方向均画线）
    const int cx = cellSize / 2;
    const int cy = cellSize / 2;
    const int half = cellSize / 2 - 2;

    painter->drawLine(cx - half, cy, cx + half, cy);   // 水平
    painter->drawLine(cx, cy - half, cx, cy + half);   // 垂直

    // 中心圆点
    painter->setBrush(color);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(cx, cy), cellSize / 5, cellSize / 5);
}
