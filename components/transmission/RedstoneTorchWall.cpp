#include "RedstoneTorchWall.h"

#include "core/model/GridModel.h"

#include <QPainter>

RedstoneTorchWall::RedstoneTorchWall(int x, int y)
    : RedstoneTorchBase(x, y,
                        Direction::South,
                        {RelDir::Back},                           // 输入：来自附着方块
                        {RelDir::Front, RelDir::Right,            // 四方向输出
                         RelDir::Back,  RelDir::Left})
{
    // 默认朝向 South：火把朝南，附着面在 Back=North（上方）
}

void RedstoneTorchWall::computeOutput(GridModel *grid)
{
    // ─── 附着校验 ───
    Direction attachDir = toAbsoluteDir(RelDir::Back, facing());
    int ax = x() + dx(attachDir), ay = y() + dy(attachDir);
    if (grid->isValid(ax, ay)) {
        auto *attached = grid->cellAt(ax, ay);
        if (!attached || !attached->isSolid()) {
            m_markedForRemoval = true;
            setOutputStrength(0);
            return;
        }
    } else {
        m_markedForRemoval = true;
        setOutputStrength(0);
        return;
    }

    RedstoneTorchBase::computeOutput(grid);
}

void RedstoneTorchWall::paintContent(QPainter *painter, int cellSize) const
{
    const int cx = cellSize / 2;

    // ─── 木棍（PERU 色，从中心延伸向附着面一侧） ───
    // 朝北版本：附着面 Back=North 在上方，木棍画在下半，
    // 基类按 facing 旋转后木棍即贴墙。
    const int stickW = cellSize / 8;                  // 2/16
    const int stickX = cx - stickW / 2;
    const int stickY = cellSize / 2;                  // 从中心到下边缘
    const int stickH = cellSize / 2 - 1;              // 留 1px 边距

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(205, 133, 63));          // PERU 木质色
    painter->drawRect(stickX, stickY, stickW, stickH);

    // ─── 火焰同心圆（居中，EGE 亮心画法） ───
    const int outerR = cellSize / 4;
    const int innerR = cellSize / 8;
    if (isLit()) {
        painter->setBrush(QColor(255, 0, 0));
        painter->drawEllipse(QPointF(cx, cellSize / 2), outerR, outerR);
        painter->setBrush(QColor(255, 255, 0));
        painter->drawEllipse(QPointF(cx, cellSize / 2), innerR, innerR);
    } else {
        painter->setBrush(QColor(135, 0, 0));
        painter->drawEllipse(QPointF(cx, cellSize / 2), outerR, outerR);
        painter->setBrush(QColor(49, 26, 17));
        painter->drawEllipse(QPointF(cx, cellSize / 2), innerR, innerR);
    }
}
