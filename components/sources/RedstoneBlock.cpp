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
    const int m = cellSize / 8;    // margin

    // 主体 — 深红色块
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(180, 40, 40));
    painter->drawRect(m, m, cellSize - m * 2, cellSize - m * 2);

    // 表面纹理 — 浅色菱形斑纹
    painter->setBrush(QColor(200, 70, 60));
    const int cx = cellSize / 2;
    const int cy = cellSize / 2;
    QPolygonF diamond;
    diamond << QPointF(cx, cy - cellSize / 4);
    diamond << QPointF(cx + cellSize / 5, cy);
    diamond << QPointF(cx, cy + cellSize / 4);
    diamond << QPointF(cx - cellSize / 5, cy);
    painter->drawPolygon(diamond);

    // 边框
    painter->setPen(QPen(QColor(120, 20, 20), 1.5));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m, m, cellSize - m * 2, cellSize - m * 2);
}
