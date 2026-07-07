#include "GridModel.h"

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
    m_grid[x][y] = std::move(comp);
}

std::unique_ptr<Component> GridModel::removeComponentAt(int x, int y)
{
    if (!isValid(x, y))
        return nullptr;
    return std::move(m_grid[x][y]);
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
