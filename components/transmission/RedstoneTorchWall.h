#pragma once

#include "RedstoneTorchBase.h"

class QPainter;

/**
 * @brief 附着红石火把 — 贴附在实心方块侧面
 *
 * 输入端口位于 Back（附着实心方块方向），
 * 四方向输出弱充能 15。
 * 每 tick 校验附着关系，若附着的不是实心方块则自销毁。
 */
class RedstoneTorchWall : public RedstoneTorchBase
{
public:
    explicit RedstoneTorchWall(int x, int y);

    void paintContent(QPainter *painter, int cellSize) const override;
    void computeOutput(GridModel *grid) override;

    bool isMarkedForRemoval() const override { return m_markedForRemoval; }

private:
    bool m_markedForRemoval = false;
};
