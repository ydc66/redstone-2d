#include "RedstoneBlock.h"

#include <QPainter>

RedstoneBlock::RedstoneBlock(int x, int y)
    : Component(x, y,
                Direction::North,
                {},                                            // 无输入端口
                {RelDir::Front, RelDir::Right,
                 RelDir::Back,  RelDir::Left})                  // 四方向输出
{
}

void RedstoneBlock::paintContent(QPainter *painter, int cellSize) const
{
    const int m = cellSize / 16;    // 1/16 内缩

    // 外层：亮红铺满整格
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(230, 32, 8));
    painter->drawRect(0, 0, cellSize, cellSize);

    // 内层：深红 1/16 内缩（EGE 双层嵌套）
    painter->setBrush(QColor(115, 12, 0));
    painter->drawRect(m, m, cellSize - m * 2, cellSize - m * 2);
}
