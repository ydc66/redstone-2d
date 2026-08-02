#pragma once

#include "core/meta_component/Component.h"

#include <QColor>

/**
 * @brief 实心方块 — 纯方块渲染 + 材质切换交互
 *
 * 直接继承 Component，自声明为 Category::Solid。
 * 内部维护材质切换状态，每次 onInteract() 切换到下一种材质。
 */
class SolidBlock : public Component
{
public:
    explicit SolidBlock(int x, int y);

    Category category() const override { return Category::Solid; }
    bool     isTransceiver() const override { return true; }

    void computeOutput(GridModel *grid) override;
    bool isStrongOutput() const override { return m_strongPowered; }
    void paintContent(QPainter *painter, int cellSize) const override;
    void onInteract() override;

private:
    struct Material {
        QColor fill;
        QColor border;
    };

    static const Material kMaterials[];
    static constexpr int   kMaterialCount = 3;

    int m_currentIdx = 0;
    bool m_strongPowered = false;
};
