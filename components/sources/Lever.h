#pragma once

#include "core/meta_component/Component.h"

class QPainter;

/**
 * @brief 拉杆 — 交互式信号源
 *
 * 点击切换 ON/OFF：
 *   - ON：向四方向输出强充能 15（红光提示）
 *   - OFF：无输出
 */
class Lever : public Component
{
public:
    explicit Lever(int x, int y);

    Category category()           const override { return Category::NonSolid; }
    bool     isSignalSource()     const override { return true; }
    bool     isStrongOutput()     const override { return true; }

    int  basePowerLevel()         const override { return m_powered ? 15 : 0; }

    void onInteract()             override { m_powered = !m_powered; }
    void paintContent(QPainter *painter, int cellSize) const override;

    bool isPowered() const { return m_powered; }

private:
    bool m_powered = false;
};
