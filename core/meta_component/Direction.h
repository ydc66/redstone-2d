#pragma once

#include <array>

// ══════════════════════════════════════════════════════════
//  方向系统 — 严格 2D（水平 4 方向取余模型）
//
//  ⚠️ 枚举顺序是硬契约：North=0, East=1, South=2, West=3 按
//     顺时针排列，全部取余运算依赖此顺序。改动枚举顺序会
//     静默破坏旋转/映射——已由文末 static_assert 契约组锁定。
// ══════════════════════════════════════════════════════════

/// 绝对方向（东南西北），按顺时针排列 —— 索引顺序是取余运算的前提
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

/// 水平方向总数（取余模数；枚举顺序 0..3 为顺时针）
constexpr int kDirectionCount = 4;

// ─── 纯方向运算 ──────────────────────────────────

/// 反方向（取余实现：+2 步）
constexpr Direction opposite(Direction d) noexcept
{
    if (d == Direction::None) return Direction::None;
    return static_cast<Direction>((static_cast<int>(d) + 2) % kDirectionCount);
}

/// 右旋 90°（顺时针 +1 步）
constexpr Direction rotateRight(Direction d) noexcept
{
    if (d == Direction::None) return Direction::None;
    return static_cast<Direction>((static_cast<int>(d) + 1) % kDirectionCount);
}

/// 左旋 90°（逆时针 -1 ≡ +3 步）
constexpr Direction rotateLeft(Direction d) noexcept
{
    if (d == Direction::None) return Direction::None;
    return static_cast<Direction>((static_cast<int>(d) + 3) % kDirectionCount);
}

// ─── 双向映射（绝对 ↔ 相对，统一 to 前缀命名） ─────

/// 绝对方向 → 相对方向：已知朝向 facing，绝对方向 d 是元件的哪一侧？
constexpr RelDir toRelativeDir(Direction d, Direction facing) noexcept
{
    if (d == Direction::None || facing == Direction::None)
        return RelDir::Front;  // 不会实际用到
    return static_cast<RelDir>((static_cast<int>(d) - static_cast<int>(facing) + kDirectionCount)
                               % kDirectionCount);
}

/// 相对方向 → 绝对方向：元件朝向 facing 的 side 侧是哪个绝对方向？
constexpr Direction toAbsoluteDir(RelDir side, Direction facing) noexcept
{
    if (facing == Direction::None)
        return Direction::None;
    return static_cast<Direction>((static_cast<int>(facing) + static_cast<int>(side))
                                  % kDirectionCount);
}

// ─── 相对方向运算 ──────────────────────────────────

/// 相对方向反侧：Front ↔ Back、Right ↔ Left
constexpr RelDir opposite(RelDir r) noexcept
{
    return static_cast<RelDir>((static_cast<int>(r) + 2) % kDirectionCount);
}

// ─── 坐标偏移 ──────────────────────────────────

/// 方向对应的 x 增量（东 +1 / 西 -1 / 其他 0）
constexpr int dx(Direction d) noexcept
{
    switch (d) {
    case Direction::East:  return 1;
    case Direction::West:  return -1;
    default:               return 0;
    }
}

/// 方向对应的 y 增量（南 +1 / 北 -1 / 其他 0；屏幕坐标 Y 向下）
constexpr int dy(Direction d) noexcept
{
    switch (d) {
    case Direction::South: return 1;
    case Direction::North: return -1;
    default:               return 0;
    }
}

// ─── 枚举遍历 ──────────────────────────────────

/// 全部水平方向，按顺时针排列
constexpr std::array<Direction, 4> allDirections() noexcept
{
    return {Direction::North, Direction::East,
            Direction::South, Direction::West};
}

// ─── 编译期契约（static_assert 锁定，与 test_Direction 运行时测试互补） ───

// 枚举顺序契约：取余运算依赖 0..3 顺时针排列
static_assert(static_cast<int>(Direction::North) == 0, "方向契约：North 索引必须为 0");
static_assert(static_cast<int>(Direction::East)  == 1, "方向契约：East 索引必须为 1");
static_assert(static_cast<int>(Direction::South) == 2, "方向契约：South 索引必须为 2");
static_assert(static_cast<int>(Direction::West)  == 3, "方向契约：West 索引必须为 3");
static_assert(static_cast<int>(Direction::None)  == 4, "方向契约：None 索引必须为 4");

// 反向：具体值表
static_assert(opposite(Direction::North) == Direction::South);
static_assert(opposite(Direction::East)  == Direction::West);
static_assert(opposite(Direction::South) == Direction::North);
static_assert(opposite(Direction::West)  == Direction::East);
static_assert(opposite(Direction::None)  == Direction::None);

// 旋转：顺时针/逆时针具体值表
static_assert(rotateRight(Direction::North) == Direction::East);
static_assert(rotateRight(Direction::East)  == Direction::South);
static_assert(rotateRight(Direction::South) == Direction::West);
static_assert(rotateRight(Direction::West)  == Direction::North);
static_assert(rotateLeft(Direction::North)  == Direction::West);
static_assert(rotateLeft(Direction::West)   == Direction::South);
static_assert(rotateLeft(Direction::South)  == Direction::East);
static_assert(rotateLeft(Direction::East)   == Direction::North);

// 代数恒等式：转两次 = 反向
static_assert(rotateRight(rotateRight(Direction::North)) == opposite(Direction::North));
static_assert(rotateRight(rotateRight(Direction::East))  == opposite(Direction::East));

// 相对方向反侧
static_assert(opposite(RelDir::Front) == RelDir::Back);
static_assert(opposite(RelDir::Back)  == RelDir::Front);
static_assert(opposite(RelDir::Right) == RelDir::Left);
static_assert(opposite(RelDir::Left)  == RelDir::Right);

// 双向映射互逆：toAbsoluteDir(toRelativeDir(d, facing), facing) == d
static_assert(toAbsoluteDir(toRelativeDir(Direction::North, Direction::North), Direction::North) == Direction::North);
static_assert(toAbsoluteDir(toRelativeDir(Direction::East,  Direction::North), Direction::North) == Direction::East);
static_assert(toAbsoluteDir(toRelativeDir(Direction::South, Direction::East),  Direction::East)  == Direction::South);
static_assert(toAbsoluteDir(toRelativeDir(Direction::West,  Direction::West),  Direction::West)  == Direction::West);

// 坐标偏移具体值
static_assert(dx(Direction::North) == 0  && dy(Direction::North) == -1);
static_assert(dx(Direction::East)  == 1  && dy(Direction::East)  == 0);
static_assert(dx(Direction::South) == 0  && dy(Direction::South) == 1);
static_assert(dx(Direction::West)  == -1 && dy(Direction::West)  == 0);
