#include "World.h"

#include "core/model/GridModel.h"
#include "engine/Engine.h"

World::World()
    : m_grid(std::make_unique<GridModel>())
    , m_engine(std::make_unique<Engine>())
{
}

World::~World() = default;

void World::start()
{
    m_running = true;
    // ─── 骨架阶段：后续启动定时器驱动 Engine ───
}

void World::stop()
{
    m_running = false;
    // ─── 骨架阶段：后续停止定时器 ───
}

void World::singleTick()
{
    if (!m_engine)
        return;

    m_engine->processTick();
    m_tick++;
}

bool World::isRunning() const
{
    return m_running;
}
