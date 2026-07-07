#pragma once

/// 方块类型，决定了物理交互行为
enum class Category
{
    Air,        ///< 空气，不参与任何逻辑
    Solid,      ///< 实心方块，可被强/弱充能，阻挡信号穿过
    NonSolid    ///< 非固体（红石线、火把等），信号可穿过
};

/**
 * @brief 元件的静态属性声明
 *
 * 不包含运行时状态，状态存储在 Behavior 子类中。
 */
struct ComponentSpec
{
    Category category        = Category::Air;
    bool     isPushable      = false;   ///< 是否能被推动
    int      basePowerLevel  = 0;       ///< 基础信号强度（信号源使用）
};
