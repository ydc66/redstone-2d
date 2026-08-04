#pragma once

#include "core/meta_component/Direction.h"

class QPainter;

/**
 * @brief 网格对象抽象基类
 *
 * 只负责三件事：
 *   - 网格占位（x/y 坐标）
 *   - 朝向与旋转（facing + directionToAngle）
 *   - 渲染（Template Method：paint() 处理朝向旋转，子类只画朝北版本）
 *
 * 不包含任何红石逻辑（信号、端口、交互等），由 Component 层提供。
 */
class WorldObject
{
public:
    WorldObject(int x, int y,
                Direction facing = Direction::North);

    virtual ~WorldObject() = default;

    // ─── 网格占位 ───
    int  x() const { return m_x; }
    int  y() const { return m_y; }
    void setPosition(int x, int y) { m_x = x; m_y = y; }

    // ─── 朝向 / 旋转 ───
    Direction facing()      const { return m_facing; }
    void setFacing(Direction d)    { m_facing = d; }

    /// 朝向 → 旋转角度（顺时针），用于 paint() 中的画布旋转
    static int directionToAngle(Direction d) noexcept;

    // ─── 渲染（Template Method：基类处理朝向旋转，子类只画朝北版本）───
    void paint(QPainter *painter, int cellSize) const;

protected:
    /// 子类实现此方法，按朝北方向绘制元件。基类 paint() 会根据 m_facing 旋转画布后再调用此方法。
    virtual void paintContent(QPainter *painter, int cellSize) const = 0;

private:
    int       m_x, m_y;
    Direction m_facing = Direction::North;
};
