#include "World.h"

#include <QTimer>

#include "components/registration/ComponentRegistry.h"
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

// ─── 编辑操作（唯一编辑入口） ───

bool World::placeComponent(int x, int y, const QString &componentId,
                           Direction facing)
{
    if (!m_grid->isValid(x, y) || m_grid->cellAt(x, y))
        return false;

    auto comp = ComponentRegistry::instance().create(componentId, x, y);
    if (!comp)
        return false;

    comp->setFacing(facing);
    m_grid->placeComponent(x, y, std::move(comp));
    emit componentPlaced(x, y);
    return true;
}

std::unique_ptr<Component> World::removeComponentAt(int x, int y)
{
    auto comp = m_grid->removeComponentAt(x, y);
    if (comp)
        emit componentRemoved(x, y);
    return comp;
}

bool World::moveComponent(int fromX, int fromY, int toX, int toY)
{
    auto comp = m_grid->removeComponentAt(fromX, fromY);
    if (!comp)
        return false;

    // 目标格非法/占用 → 回滚
    if (!m_grid->isValid(toX, toY) || m_grid->cellAt(toX, toY)) {
        m_grid->placeComponent(fromX, fromY, std::move(comp));
        return false;
    }

    comp->setPosition(toX, toY);
    m_grid->placeComponent(toX, toY, std::move(comp));
    emit componentRemoved(fromX, fromY);
    emit componentPlaced(toX, toY);
    return true;
}

void World::interactAt(int x, int y)
{
    auto *comp = m_grid->cellAt(x, y);
    if (!comp)
        return;

    comp->onInteract();   // SolidBlockBehavior 会切换材质
    emit componentInteracted(x, y);
}

Component* World::cellAt(int x, int y) const
{
    return m_grid->cellAt(x, y);
}

bool World::isValid(int x, int y) const
{
    return m_grid->isValid(x, y);
}

void World::resizeGrid(int w, int h)
{
    m_grid->resize(w, h);
}

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

    m_engine->processTick(m_grid.get());

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
