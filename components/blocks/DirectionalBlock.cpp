#include "DirectionalBlock.h"

#include <QPainter>
#include <QtMath>

// ═══════════════════════════════════════════════════════════
//  DirectionalBlock — 方向箭头方块
// ═══════════════════════════════════════════════════════════

DirectionalBlock::DirectionalBlock(int x, int y)
    : Component(x, y)
{
}

void DirectionalBlock::paintContent(QPainter *painter, int cellSize) const
{
    const int cs = cellSize;
    const int margin = cs / 8;            // 边距
    const int body = cs - 2 * margin;     // 箭头主体区域边长
    const int cx = cs / 2;                // 中心 x
    const int tipY = margin;              // 箭头顶端 y

    // ─── 圆角背景 ───
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#E8D8B0"));             // 米黄底
    painter->drawRoundedRect(margin, margin, body, body, cs / 6, cs / 6);

    // ─── 边框 ───
    painter->setPen(QPen(QColor("#8B7D5B"), 2));
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(margin, margin, body, body, cs / 6, cs / 6);

    // ─── 箭头（朝北，基类 paint 会根据朝向旋转） ───
    const int shaftW = qMax(cs / 8, 3);                // 箭杆宽
    const int shaftH = body / 2;                        // 箭杆高（从中心到尾部）
    const int headW  = body * 2 / 3;                    // 箭头宽
    const int headH  = body / 3;                        // 箭头高

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#C0392B"));              // 红色箭头

    // 箭头三角形（在方格上半部分，尖端朝北）
    QPolygonF arrowHead;
    arrowHead << QPointF(cx, tipY)                                          // 顶端
              << QPointF(cx - headW / 2.0, tipY + headH)                    // 左下
              << QPointF(cx + headW / 2.0, tipY + headH);                   // 右下
    painter->drawPolygon(arrowHead);

    // 箭杆（从箭头底部延伸到方格中心偏下）
    const int shaftTop = tipY + headH;
    const int shaftBot = shaftTop + shaftH;
    painter->drawRect(cx - shaftW / 2, shaftTop, shaftW, shaftH);
}

void DirectionalBlock::onInteract()
{
    setFacing(rotateRight(facing()));
}
