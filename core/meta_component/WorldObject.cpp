#include "WorldObject.h"

#include <QPainter>

WorldObject::WorldObject(int x, int y, Direction facing)
    : m_x(x)
    , m_y(y)
    , m_facing(facing)
{
}

// ═══════════════════════════════════════════════════════════
//  渲染（Template Method）
// ═══════════════════════════════════════════════════════════

int WorldObject::directionToAngle(Direction d) noexcept
{
    // North=0 → 0°, East=1 → 90°, South=2 → 180°, West=3 → 270°
    return static_cast<int>(d) * 90;
}

void WorldObject::paint(QPainter *painter, int cellSize) const
{
    painter->save();
    // 绕格子中心旋转，使朝北的绘制结果适配实际朝向
    painter->translate(cellSize / 2.0, cellSize / 2.0);
    painter->rotate(directionToAngle(m_facing));
    painter->translate(-cellSize / 2.0, -cellSize / 2.0);
    paintContent(painter, cellSize);
    painter->restore();
}
