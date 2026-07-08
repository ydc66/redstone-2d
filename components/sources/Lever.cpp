#include "Lever.h"

#include <QPainter>
#include <QRadialGradient>

Lever::Lever(int x, int y)
    : Component(x, y,
                Direction::North,
                {},                                             // 无输入
                {RelDir::Front, RelDir::Right,                  // 四方向输出
                 RelDir::Back,  RelDir::Left})
{
}

void Lever::paintContent(QPainter *painter, int cellSize) const
{
    const int cx = cellSize / 2;
    const int cy = cellSize / 2;

    // ─── 拉下时冒红光 ───
    if (m_powered) {
        // 外围光晕
        QRadialGradient glow(cx, cy, cellSize / 2);
        glow.setColorAt(0.0, QColor(255, 60, 60, 160));
        glow.setColorAt(0.6, QColor(255, 30, 30, 60));
        glow.setColorAt(1.0, QColor(255, 0, 0, 0));
        painter->setPen(Qt::NoPen);
        painter->setBrush(glow);
        painter->drawEllipse(QPointF(cx, cy), cellSize / 2, cellSize / 2);

        // 中心红点
        painter->setBrush(QColor(255, 50, 50));
        painter->drawRect(cx - cellSize / 6, cy - cellSize / 6,
                          cellSize / 3, cellSize / 3);
    }

    // ─── 拉杆底座（灰色石块） ───
    painter->setPen(QPen(QColor(60, 60, 60), 1));
    painter->setBrush(QColor(140, 140, 140));
    painter->drawRect(cellSize / 3, cy + cellSize / 8,
                      cellSize / 3, cellSize / 5);

    // ─── 拉杆手柄（棕色短棍） ───
    painter->setPen(QPen(QColor(139, 90, 43), 2));
    painter->drawLine(cx, cy + cellSize / 8,
                      cx, cy - cellSize / 4);
}
