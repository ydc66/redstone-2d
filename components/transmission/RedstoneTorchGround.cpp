#include "RedstoneTorchGround.h"

#include <QPainter>

RedstoneTorchGround::RedstoneTorchGround(int x, int y)
    : RedstoneTorchBase(x, y,
                        Direction::North,
                        {},                                     // 无输入
                        {RelDir::Front, RelDir::Right,          // 四方向输出
                         RelDir::Back,  RelDir::Left})
{
}

void RedstoneTorchGround::paintContent(QPainter *painter, int cellSize) const
{
    const int cx = cellSize / 2;
    const int cy = cellSize / 2;
    const int outerR = cellSize / 4;    // 外圈半径（4/16）
    const int innerR = cellSize / 8;    // 内圈半径（2/16）

    painter->setPen(Qt::NoPen);
    if (isLit()) {
        // 点亮：暗红外圈 + 亮黄内芯（EGE 亮心画法）
        painter->setBrush(QColor(255, 0, 0));
        painter->drawEllipse(QPointF(cx, cy), outerR, outerR);
        painter->setBrush(QColor(255, 255, 0));
        painter->drawEllipse(QPointF(cx, cy), innerR, innerR);
    } else {
        // 熄灭：暗红外圈 + 深棕内芯
        painter->setBrush(QColor(135, 0, 0));
        painter->drawEllipse(QPointF(cx, cy), outerR, outerR);
        painter->setBrush(QColor(49, 26, 17));
        painter->drawEllipse(QPointF(cx, cy), innerR, innerR);
    }
}
