#include "RedstoneLamp.h"

#include <QPainter>

RedstoneLamp::RedstoneLamp(int x, int y)
    : Component(x, y,
                Direction::North,
                {RelDir::Front, RelDir::Right,              // 四方向输入
                 RelDir::Back,  RelDir::Left},
                {})                                          // 无输出
{
}

void RedstoneLamp::paintContent(QPainter *painter, int cellSize) const
{
    const int m = cellSize / 8;

    if (m_lit) {
        // 亮起 — 亮黄色
        painter->setPen(QPen(QColor(220, 180, 0), 2));
        painter->setBrush(QColor(255, 230, 80));
    } else {
        // 熄灭 — 暗红色
        painter->setPen(QPen(QColor(80, 40, 20), 2));
        painter->setBrush(QColor(100, 60, 30));
    }

    painter->drawRoundedRect(m, m, cellSize - m * 2, cellSize - m * 2, 4, 4);

    // 灯芯圆点
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_lit ? QColor(255, 250, 200) : QColor(60, 40, 20));
    const int cx = cellSize / 2;
    const int cy = cellSize / 2;
    painter->drawEllipse(QPointF(cx, cy), cellSize / 5, cellSize / 5);
}

void RedstoneLamp::onTick(const std::array<RedstoneSignal, 4>& sigArray)
{
    bool powered = false;
    for (Direction dir : allDirections()) {
        if (!canInputFrom(dir))
            continue;
        if (sigArray[static_cast<int>(dir)].strength > 0) {
            powered = true;
            break;
        }
    }
    m_lit = powered;
}
