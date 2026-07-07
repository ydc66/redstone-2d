#include "SolidBlock.h"

#include "core/model/GridModel.h"
#include "core/meta_component/Direction.h"

#include <QPainter>

// ═══════════════════════════════════════════════════════════
//  材质预设表
// ═══════════════════════════════════════════════════════════

const SolidBlock::Material SolidBlock::kMaterials[] = {
    { QColor("#8B8B8B"), QColor("#6B6B6B"), QColor("#7A7A7A") },   // 灰石砖
    { QColor("#A0A0A0"), QColor("#808080"), QColor("#909090") },   // 石砖
    { QColor("#C0A070"), QColor("#A08050"), QColor("#B09060") },   // 土砖
};

// ═══════════════════════════════════════════════════════════
//  SolidBlock — 实心方块
// ═══════════════════════════════════════════════════════════

SolidBlock::SolidBlock(int x, int y)
    : Component(x, y,
                Direction::North,
                {RelDir::Front, RelDir::Right,                   // 四方向输入（被充能）
                 RelDir::Back,  RelDir::Left},
                {RelDir::Front, RelDir::Right,                   // 四方向输出（传导弱充能）
                 RelDir::Back,  RelDir::Left})
    , m_currentIdx(0)
{
}

void SolidBlock::computeOutput(GridModel *grid)
{
    int maxInput = 0;
    m_strongPowered = false;
    for (Direction dir : {Direction::North, Direction::East,
                          Direction::South, Direction::West}) {
        RedstoneSignal sig = grid->signalFrom(x(), y(), dir);
        if (sig.strength > maxInput)
            maxInput = sig.strength;
        if (sig.isStrong)
            m_strongPowered = true;
    }
    // 实心方块不衰减传导（弱充能）
    setOutputStrength(maxInput);
}

void SolidBlock::paintContent(QPainter *painter, int cellSize) const
{
    const auto &mat = kMaterials[m_currentIdx];

    // 填充主体色
    painter->fillRect(0, 0, cellSize, cellSize, mat.fill);

    // 边框
    painter->setPen(QPen(mat.border, 2));
    painter->drawRect(1, 1, cellSize - 2, cellSize - 2);

    // 纹理（对角线交叉线）
    if (mat.texture.isValid()) {
        QPen texPen(mat.texture, 1);
        painter->setPen(texPen);
        painter->drawLine(2, 2, cellSize - 2, cellSize - 2);
        painter->drawLine(cellSize - 2, 2, 2, cellSize - 2);
    }
}

void SolidBlock::onInteract()
{
    m_currentIdx = (m_currentIdx + 1) % kMaterialCount;
}
