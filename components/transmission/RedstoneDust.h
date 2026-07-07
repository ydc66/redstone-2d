#pragma once

#include "core/meta_component/Component.h"

class QPainter;

/**
 * @brief 红石粉 — 传输元件，信号衰减传播
 *
 * 读取 4 方向邻居的强充能信号的最大值，输出 max-1（最低 0）。
 * 弱充能不可激活红石粉。
 * 自身输出为强充能，可继续激活下游红石粉。
 *
 * 所有 4 个方向均可输入/输出（全向连接）。
 */
class RedstoneDust : public Component
{
public:
    explicit RedstoneDust(int x, int y);

    Category category()       const override { return Category::NonSolid; }
    bool     isTransceiver()  const override { return true; }
    bool     isStrongOutput() const override { return true; }

    void computeOutput(GridModel *grid) override;
    void paintContent(QPainter *painter, int cellSize) const override;
};
