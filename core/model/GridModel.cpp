#include "GridModel.h"

// ─── GridModel ───

GridModel::GridModel()
{
}

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
    return m_grid[x][y];
}

void GridModel::placeComponent(int x, int y, Component *comp)
{
    if (!isValid(x, y))
        return;
    m_grid[x][y] = comp;
}

Component* GridModel::removeComponentAt(int x, int y)
{
    if (!isValid(x, y))
        return nullptr;
    Component *comp = m_grid[x][y];
    m_grid[x][y] = nullptr;
    return comp;
}
