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

    // ─── 拉下时冒红光（保留：旋转后仍可区分通电状态） ───
    if (m_powered) {
        // 外围光晕
        QRadialGradient glow(cx, cellSize / 2, cellSize / 2);
        glow.setColorAt(0.0, QColor(255, 60, 60, 160));
        glow.setColorAt(0.6, QColor(255, 30, 30, 60));
        glow.setColorAt(1.0, QColor(255, 0, 0, 0));
        painter->setPen(Qt::NoPen);
        painter->setBrush(glow);
        painter->drawEllipse(QPointF(cx, cellSize / 2), cellSize / 2, cellSize / 2);

        // 中心红点
        painter->setBrush(QColor(255, 50, 50));
        painter->drawRect(cx - cellSize / 6, cellSize / 2 - cellSize / 6,
                          cellSize / 3, cellSize / 3);
    }

    // ─── 拉杆底座（灰色石块，EGE 风格） ───
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(128, 128, 128));
    painter->drawRect(cellSize * 5 / 16, cellSize * 4 / 16,
                      cellSize * 6 / 16, cellSize * 8 / 16);

    // ─── 拉杆手柄（PERU 木柄，2/16 宽） ───
    // 未激活：柄朝上（上半格）；激活：柄朝下（下半格）
    painter->setBrush(QColor(205, 133, 63));
    const int stickY = m_powered ? cellSize * 8 / 16 : cellSize * 2 / 16;
    painter->drawRect(cellSize * 7 / 16, stickY,
                      cellSize * 2 / 16, cellSize * 6 / 16);
}
