#include "SolidBlockBehavior.h"

#include <QPainter>

// ═══════════════════════════════════════════════════════════
//  材质预设表
// ═══════════════════════════════════════════════════════════

const SolidBlockBehavior::Material SolidBlockBehavior::kMaterials[] = {
    { QColor("#8B8B8B"), QColor("#6B6B6B"), QColor("#7A7A7A") },   // 灰石砖
    { QColor("#A0A0A0"), QColor("#808080"), QColor("#909090") },   // 石砖
    { QColor("#C0A070"), QColor("#A08050"), QColor("#B09060") },   // 土砖
};

// ═══════════════════════════════════════════════════════════
//  SolidBlockBehavior — 实心方块
// ═══════════════════════════════════════════════════════════

SolidBlockBehavior::SolidBlockBehavior()
    : m_currentIdx(0)
{
}

void SolidBlockBehavior::paint(QPainter *painter, int cellSize) const
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

void SolidBlockBehavior::onInteract()
{
    m_currentIdx = (m_currentIdx + 1) % kMaterialCount;
}

// ═══════════════════════════════════════════════════════════
//  工厂函数
// ═══════════════════════════════════════════════════════════

std::unique_ptr<Component> createSolidBlock(int x, int y)
{
    ComponentSpec spec;
    spec.category       = Category::Solid;
    spec.isPushable     = false;
    spec.basePowerLevel = 0;

    return std::make_unique<Component>(
        x, y,
        Direction::North,
        spec,
        QList<RelDir>{},
        QList<RelDir>{},
        std::make_unique<SolidBlockBehavior>());
}
