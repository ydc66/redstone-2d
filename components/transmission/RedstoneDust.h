#pragma once

#include "core/meta_component/Component.h"

class QPainter;

/**
 * @brief 红石粉 — 传输元件，信号衰减传播
 *
 * 读取 4 方向邻居信号的最大值，输出 max-1（最低 0）。
 * 若输出发生变化则继续 BFS 传播。
 *
 * 所有 4 个方向均可输入/输出（全向连接）。
 */
class RedstoneDust : public Component
{
public:
    explicit RedstoneDust(int x, int y);

    Category category()       const override { return Category::NonSolid; }
    bool     isTransceiver()  const override { return true; }

    void computeOutput(World *world) override;
    void paintContent(QPainter *painter, int cellSize) const override;
};
