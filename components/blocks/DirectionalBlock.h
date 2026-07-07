#pragma once

#include "core/meta_component/Component.h"

class QPainter;

/**
 * @brief 方向箭头方块 — 显示箭头指向当前朝向，右键旋转 90°
 *
 * 直接继承 Component，自声明为 Category::NonSolid。
 * 箭头朝北绘制，基类 paint() 会根据 m_facing 旋转画布以使箭头指向实际朝向。
 * 每次 onInteract() 顺时针旋转 90°。
 */
class DirectionalBlock : public Component
{
public:
    explicit DirectionalBlock(int x, int y);

    Category category() const override { return Category::NonSolid; }

    void paintContent(QPainter *painter, int cellSize) const override;
    void onInteract() override;
};
