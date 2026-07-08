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
    Direction attachDir = resolveDir(RelDir::Back, facing());
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

    // ─── 木棍（棕色长方形，位于下方） ───
    const int stickW = cellSize / 6;
    const int stickX = cx - stickW / 2;
    const int stickY = cellSize / 2;                  // 从中间到下方
    const int stickH = cellSize - stickY - 1;         // 留 1px 边距

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(139, 90, 43));
    painter->drawRect(stickX, stickY, stickW, stickH);

    // ─── 火焰（小正方形，位于上方） ───
    const int sqSize = cellSize / 3;
    const int sqX = (cellSize - sqSize) / 2;
    const int sqY = cellSize / 6;                     // 靠上位置

    painter->setBrush(isLit() ? QColor(220, 50, 50)     // 点亮：红色
                              : QColor(100, 20, 20));   // 熄灭：暗红色
    painter->drawRect(sqX, sqY, sqSize, sqSize);
}
