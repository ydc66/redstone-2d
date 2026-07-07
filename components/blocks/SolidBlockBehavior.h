#pragma once

#include "core/types/Behavior.h"
#include "core/types/Component.h"

#include <QColor>
#include <memory>

class QPainter;

/**
 * @brief 实心方块行为 — 纯方块渲染
 *
 * 内部维护材质切换状态，每次 onInteract() 切换到下一种材质。
 * 信号/Tick 等引擎功能为空，后续按需实现。
 */
class SolidBlockBehavior : public Behavior
{
public:
    explicit SolidBlockBehavior();

    void paint(QPainter *painter, int cellSize) const override;
    void onInteract() override;

private:
    /// 材质预设
    struct Material {
        QColor fill;
        QColor border;
        QColor texture;
    };

    static const Material kMaterials[];   ///< 所有材质列表
    static constexpr int   kMaterialCount = 3;

    int m_currentIdx = 0;                ///< 当前材质下标
};

/// 创建实心方块元件的工厂函数
std::unique_ptr<Component> createSolidBlock(int x, int y);
