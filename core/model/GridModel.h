#pragma once

#include <memory>
#include <vector>

// 前向声明
class Component;

/// 网格模型，管理网格布局（单层：每个格子最多一个元件）
class GridModel
{
public:
    GridModel();
    ~GridModel();

    GridModel(const GridModel&) = delete;
    GridModel& operator=(const GridModel&) = delete;

    // ─── 尺寸 ───
    int width()  const { return m_width; }
    int height() const { return m_height; }
    bool isValid(int x, int y) const;
    void resize(int w, int h);

    // ─── 格子操作 ───
    Component* cellAt(int x, int y) const;
    void placeComponent(int x, int y, Component *comp);
    Component* removeComponentAt(int x, int y);

private:
    int m_width  = 0;
    int m_height = 0;

    std::vector<std::vector<Component*>>  m_grid;  ///< 网格数据
};
