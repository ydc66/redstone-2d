#pragma once

#include <array>

/// 方向（东南西北）
enum class Direction {
    North, South, East, West, None
};

/// 获取反方向
constexpr Direction opposite(Direction d) noexcept
{
    switch (d) {
    case Direction::North: return Direction::South;
    case Direction::South: return Direction::North;
    case Direction::East:  return Direction::West;
    case Direction::West:  return Direction::East;
    default:               return Direction::None;
    }
}

/// 获取方向的 x 偏移量
constexpr int dx(Direction d) noexcept
{
    switch (d) {
    case Direction::East:  return 1;
    case Direction::West:  return -1;
    default:               return 0;
    }
}

/// 获取方向的 y 偏移量
constexpr int dy(Direction d) noexcept
{
    switch (d) {
    case Direction::South: return 1;
    case Direction::North: return -1;
    default:               return 0;
    }
}

/// 获取所有方向
inline std::array<Direction, 4> allDirections() noexcept
{
    return {Direction::North, Direction::South,
            Direction::East,  Direction::West};
}
