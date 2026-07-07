#pragma once

#include <array>
#include <type_traits>

/// 绝对方向（东南西北），按顺时针排列
enum class Direction : int {
    North = 0,
    East  = 1,
    South = 2,
    West  = 3,
    None  = 4
};

/// 相对方向（相对于元件的朝向）
enum class RelDir : int {
    Front = 0,  ///< 正面 = m_facing
    Right = 1,  ///< 右侧 = 顺时针 90°
    Back  = 2,  ///< 背面 = opposite
    Left  = 3   ///< 左侧 = 逆时针 90°
};

// ─── 纯方向运算 ──────────────────────────────────

/// 反方向（取余实现）
constexpr Direction opposite(Direction d) noexcept
{
    if (d == Direction::None) return Direction::None;
    return static_cast<Direction>((static_cast<int>(d) + 2) % 4);
}

/// 右旋 90°（顺时针 +1）
constexpr Direction rotateRight(Direction d) noexcept
{
    if (d == Direction::None) return Direction::None;
    return static_cast<Direction>((static_cast<int>(d) + 1) % 4);
}

/// 左旋 90°（逆时针 -1 ≡ +3）
constexpr Direction rotateLeft(Direction d) noexcept
{
    if (d == Direction::None) return Direction::None;
    return static_cast<Direction>((static_cast<int>(d) + 3) % 4);
}

// ─── 双向映射（绝对 ↔ 相对） ─────────────────────

/// 绝对方向 → 相对方向：已知朝向 facing，绝对方向 d 是元件的哪一侧？
inline RelDir toRelativeDir(Direction d, Direction facing) noexcept
{
    if (d == Direction::None || facing == Direction::None)
        return RelDir::Front;  // 不会实际用到
    return static_cast<RelDir>((static_cast<int>(d) - static_cast<int>(facing) + 4) % 4);
}

/// 相对方向 → 绝对方向：元件朝向 facing 的 side 侧是哪个绝对方向？
inline Direction resolveDir(RelDir side, Direction facing) noexcept
{
    if (facing == Direction::None)
        return Direction::None;
    return static_cast<Direction>((static_cast<int>(facing) + static_cast<int>(side)) % 4);
}

// ─── 坐标偏移 ──────────────────────────────────

constexpr int dx(Direction d) noexcept
{
    switch (d) {
    case Direction::East:  return 1;
    case Direction::West:  return -1;
    default:               return 0;
    }
}

constexpr int dy(Direction d) noexcept
{
    switch (d) {
    case Direction::South: return 1;
    case Direction::North: return -1;
    default:               return 0;
    }
}

// ─── 枚举遍历 ──────────────────────────────────

inline std::array<Direction, 4> allDirections() noexcept
{
    return {Direction::North, Direction::East,
            Direction::South, Direction::West};
}
