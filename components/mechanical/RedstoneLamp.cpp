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
    const int m = cellSize / 16;    // 1/16 内缩

    // 外框：深棕铺满整格
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(49, 26, 17));
    painter->drawRect(0, 0, cellSize, cellSize);

    // 内层：亮/灭双色（EGE 直角双层）
    painter->setBrush(m_lit ? QColor(243, 192, 121)     // 亮：暖黄
                            : QColor(134, 78, 41));      // 灭：棕红
    painter->drawRect(m, m, cellSize - m * 2, cellSize - m * 2);
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
