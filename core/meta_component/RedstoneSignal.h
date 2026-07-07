#pragma once

#include "Direction.h"

/**
 * @brief 红石信号 — 完整描述一个方向上的输入信号
 *
 * 包含强度、传播方向、充能类型。
 * 命名避开 Qt 的 signal 关键字以免混淆。
 *
 * 消费方根据 isStrong 决定是否可用（弱充能不能激活传输元件）。
 * direction 为信号的传播方向（指向的方向）。
 */
struct RedstoneSignal
{
    int       strength  = 0;                    ///< 信号强度 0-15
    Direction direction = Direction::None;       ///< 信号传播方向（指向方向）
    bool      isStrong  = false;                ///< 是否为强充能
};
