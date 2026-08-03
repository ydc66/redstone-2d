#include "GridModel.h"

#include <algorithm>

#include "core/meta_component/Component.h"

// ─── GridModel ───

GridModel::GridModel() = default;

GridModel::~GridModel() = default;

void GridModel::resize(int w, int h)
{
    m_width  = w;
    m_height = h;

    m_grid.resize(w);
    for (int x = 0; x < w; ++x)
        m_grid[x].resize(h);

    // 尺寸变化后重建活动列表（覆盖扩大/缩小场景）
    m_active.clear();
    for (int x = 0; x < w; ++x)
        for (int y = 0; y < h; ++y)
            if (m_grid[x][y])
                m_active.push_back(m_grid[x][y].get());
}

bool GridModel::isValid(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

Component* GridModel::cellAt(int x, int y) const
{
    if (!isValid(x, y))
        return nullptr;
    return m_grid[x][y].get();
}

void GridModel::placeComponent(int x, int y, std::unique_ptr<Component> comp)
{
    if (!isValid(x, y))
        return;

    // 覆盖放置：旧元件移出活动列表，防止悬挂指针
    if (m_grid[x][y]) {
        auto it = std::find(m_active.begin(), m_active.end(), m_grid[x][y].get());
        if (it != m_active.end())
            m_active.erase(it);
    }

    m_grid[x][y] = std::move(comp);
    m_active.push_back(m_grid[x][y].get());
}

std::unique_ptr<Component> GridModel::removeComponentAt(int x, int y)
{
    if (!isValid(x, y))
        return nullptr;

    auto comp = std::move(m_grid[x][y]);
    if (comp) {
        auto it = std::find(m_active.begin(), m_active.end(), comp.get());
        if (it != m_active.end())
            m_active.erase(it);
    }
    return comp;
}

RedstoneSignal GridModel::signalFrom(int x, int y, Direction fromDir) const
{
    // 邻居坐标 = 从 fromDir 方向来的那个邻居
    int nx = x + dx(fromDir);
    int ny = y + dy(fromDir);

    if (!isValid(nx, ny))
        return {};

    Component *neighbor = cellAt(nx, ny);
    if (!neighbor)
        return {};

    // 邻居必须能向对侧（朝向本格）输出
    Direction signalDir = opposite(fromDir);    // 信号指向方向 = 朝向我们
    if (!neighbor->canOutputTo(signalDir))
        return {};

    return RedstoneSignal{
        .strength  = neighbor->outputStrength(),
        .direction = signalDir,                 // 传播方向 = 指向本格
        .isStrong  = neighbor->isStrongOutput()
    };
}
