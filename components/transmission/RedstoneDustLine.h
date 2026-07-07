#pragma once

#include "RedstoneDust.h"

class QPainter;

/**
 * @brief 一字红石粉 — 直线传输
 *
 * 仅两端（Front / Back）可输入/输出，方向由 facing 决定：
 * - facing = North / South → 竖直导线
 * - facing = East / West  → 水平导线
 *
 * 渲染：朝北绘制竖线，基类 paint() 自动旋转适配实际朝向。
 * 右键点击顺时针旋转 90°。
 */
class RedstoneDustLine : public RedstoneDust
{
public:
    explicit RedstoneDustLine(int x, int y);

    void paintContent(QPainter *painter, int cellSize) const override;
    void onInteract() override;
};
