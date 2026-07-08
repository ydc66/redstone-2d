#pragma once

#include "RedstoneTorchBase.h"

class QPainter;

/**
 * @brief 地基红石火把 — 永远点亮，无输入端口
 *
 * 四方向输出弱充能 15。纯粹的信号源角色，
 * 不接收任何外界信号，永不熄灭。
 */
class RedstoneTorchGround : public RedstoneTorchBase
{
public:
    explicit RedstoneTorchGround(int x, int y);

    void paintContent(QPainter *painter, int cellSize) const override;
};
