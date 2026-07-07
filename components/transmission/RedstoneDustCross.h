#pragma once

#include "RedstoneDust.h"

class QPainter;

/**
 * @brief 十字红石粉 — 四向交叉
 *
 * 四个方向均可输入/输出，facing 不影响功能。
 * 渲染：朝北绘制十字，旋转后仍为十字。
 */
class RedstoneDustCross : public RedstoneDust
{
public:
    explicit RedstoneDustCross(int x, int y);

    void paintContent(QPainter *painter, int cellSize) const override;
};
