#include "World.h"

#include <QTimer>

#include "core/meta_component/Component.h"
#include "core/model/GridModel.h"
#include "core/engine/Engine.h"

World::World(QObject *parent)
    : QObject(parent)
    , m_grid(std::make_unique<GridModel>())
    , m_engine(std::make_unique<Engine>())
{
    // 创建定时器
    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &World::onTimerTick);
}

World::~World() = default;

void World::start()
{
    if (m_running) return;
    m_running = true;

    // 根据速度倍率计算间隔：100ms / speed
    int interval = static_cast<int>(100.0 / m_speed);
    m_timer->start(std::max(interval, 10));
}

void World::stop()
{
    if (!m_running) return;
    m_running = false;
    m_timer->stop();
}

void World::singleTick()
{
    if (!m_engine || !m_grid)
        return;

    m_engine->processTick(this);

    // ─── 清理标记为待销毁的元件 ───
    for (int x = 0; x < m_grid->width(); ++x) {
        for (int y = 0; y < m_grid->height(); ++y) {
            auto *comp = m_grid->cellAt(x, y);
            if (comp && comp->isMarkedForRemoval()) {
                m_grid->removeComponentAt(x, y);
            }
        }
    }

    m_tick++;
    emit tickCompleted(m_tick);
}

void World::setSpeed(double speed)
{
    m_speed = speed;
    if (m_running) {
        // 运行中调整速度 → 重启定时器
        m_timer->stop();
        int interval = static_cast<int>(100.0 / m_speed);
        m_timer->start(std::max(interval, 10));
    }
}

void World::onTimerTick()
{
    singleTick();
}
