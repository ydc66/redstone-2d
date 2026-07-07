#pragma once

#include "core/meta_component/Component.h"

class QPainter;

/**
 * @brief 红石粉抽象基类 — 共享衰减传播逻辑
 *
 * computeOutput() 自动根据子类构造时设定的 m_inputPorts 遍历对应绝对方向，
 * 读取邻居信号并取最大值，输出 max-1。弱充能过滤规则：
 * - 实心方块的弱充能不能激活红石粉（仅强充能可以）
 * - 其他元件（含同级红石粉）的弱充能可以激活红石粉（直连有效）
 *
 * 子类只需在构造函数中设定 ports，并实现 paintContent()。
 */
class RedstoneDust : public Component
{
public:
    Category category()       const override { return Category::NonSolid; }
    bool     isTransceiver()  const override { return true; }
    bool     isStrongOutput() const override { return false; }

    void computeOutput(GridModel *grid) override;

protected:
    /// 子类通过此构造函数设定端口列表
    RedstoneDust(int x, int y, Direction facing,
                 QList<RelDir> inputPorts,
                 QList<RelDir> outputPorts);
};
