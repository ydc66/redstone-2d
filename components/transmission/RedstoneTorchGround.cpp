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
    const int size = cellSize / 3;                  // 小正方形
    const int x = (cellSize - size) / 2;
    const int y = (cellSize - size) / 2;

    painter->setPen(Qt::NoPen);
    painter->setBrush(isLit() ? QColor(220, 50, 50)     // 点亮：红色
                              : QColor(100, 20, 20));   // 熄灭：暗红色
    painter->drawRect(x, y, size, size);
}
