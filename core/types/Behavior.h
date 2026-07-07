#pragma once

class QPainter;

/**
 * @brief 元件行为策略基类
 *
 * 当前骨架阶段只保留渲染接口，
 * 信号/Tick 等引擎功能将在后续实现。
 */
class Behavior
{
public:
    virtual ~Behavior() = default;

    // ─── 渲染 ───
    virtual void paint(QPainter *painter, int cellSize) const = 0;

    // ─── 交互（子类可响应点击切换状态） ───
    virtual void onInteract() {}
};
