#pragma once

#include "core/meta_component/Component.h"

class QPainter;

/**
 * @brief 红石灯 — 消费者，被充能时亮起
 *
 * 四方向均可接收信号，通过 port 系统 + canInputFrom() 过滤。
 * 无输出端口。
 */
class RedstoneLamp : public Component
{
public:
    explicit RedstoneLamp(int x, int y);

    Category category()       const override { return Category::NonSolid; }
    bool     isConsumer()     const override { return true; }

    void paintContent(QPainter *painter, int cellSize) const override;
    void onTick(const std::array<RedstoneSignal, 4>& sigArray) override;

    bool isLit() const { return m_lit; }

private:
    bool m_lit = false;
};
