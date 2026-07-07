#pragma once

#include "core/meta_component/Component.h"

class QPainter;

/**
 * @brief 红石块 — 信号源，始终输出强度 15
 *
 * 无输入端口，所有 4 个方向均输出强度 15 信号。
 * 在 Phase 1 中被设为信号源，Phase 2 无需额外计算。
 */
class RedstoneBlock : public Component
{
public:
    explicit RedstoneBlock(int x, int y);

    Category category()           const override { return Category::NonSolid; }
    bool     isSignalSource()     const override { return true; }
    int      basePowerLevel()     const override { return 15; }

    void paintContent(QPainter *painter, int cellSize) const override;
};
