/**
 * @file   InteractionManager.cpp
 * @brief  交互事件分发核心实现
 */

#include "InteractionManager.h"

#include "interaction_mode/ViewInteraction.h"
#include "interaction_mode/SelectInteraction.h"
#include "interaction_mode/PlaceInteraction.h"
#include "interaction_mode/InteractInteraction.h"
#include "interaction_mode/DestroyInteraction.h"

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QWidget>

InteractionManager::InteractionManager(QGraphicsView *view,
                                         GridModel *grid,
                                         GridGraphicsScene *scene,
                                         QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_grid(grid)
    , m_scene(scene)
{
    /// 创建各种交互模式实例
    m_interactions[static_cast<int>(InteractionMode::View)]     = new ViewInteraction(grid, scene, view, this);
    m_interactions[static_cast<int>(InteractionMode::Select)]   = new SelectInteraction(grid, scene, view, this);
    m_interactions[static_cast<int>(InteractionMode::Place)]    = new PlaceInteraction(grid, scene, view, this);
    m_interactions[static_cast<int>(InteractionMode::Interact)] = new InteractInteraction(grid, scene, view, this);
    m_interactions[static_cast<int>(InteractionMode::Destroy)]  = new DestroyInteraction(grid, scene, view, this);
}

void InteractionManager::install()
{
    if (m_view)
        m_view->viewport()->installEventFilter(this);
}

void InteractionManager::remove()
{
    if (m_view)
        m_view->viewport()->removeEventFilter(this);
}

void InteractionManager::setMode(InteractionMode mode)
{
    if (mode == m_currentMode)
        return;

    // 如果当前有活动的交互模式，则调用其 退出模式 方法
    if (activeInteraction())
        activeInteraction()->onDeactivate();

    // 切换当前模式
    m_currentMode = mode;

    // 如果当前有活动的交互模式，则调用其 进入模式 方法
    if (activeInteraction())
        activeInteraction()->onActivate();

    // 设置视图光标
    if (m_view)
        m_view->viewport()->setCursor(
            activeInteraction() ? activeInteraction()->cursor()
                                : Qt::ArrowCursor);
        
    // 发射模式切换信号
    emit modeChanged(mode);
}

bool InteractionManager::eventFilter(QObject *obj, QEvent *event)
{   
    // 如果对象不是视图口，则调用父类的 eventFilter 方法
    if (!m_view || obj != m_view->viewport())
        return QObject::eventFilter(obj, event);

    // 根据事件类型进行处理
    switch (event->type()) {
    
    // 交互模式滚轮事件
    case QEvent::Wheel: {
        auto *we = static_cast<QWheelEvent *>(event);
        if (activeInteraction())
            activeInteraction()->onWheel(we);
        return true;
    }

    // 交互模式鼠标左键按下事件
    case QEvent::MouseButtonPress: {
        auto *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::LeftButton && activeInteraction()) {
            const QPointF scenePos = m_view->mapToScene(me->pos());
            activeInteraction()->onLeftPress(scenePos);
            return true;
        }
        return false;
    }

    /// 交互模式鼠标移动事件
    case QEvent::MouseMove: {
        if (activeInteraction()) {
            auto *me = static_cast<QMouseEvent *>(event);
            const QPointF scenePos = m_view->mapToScene(me->pos());
            activeInteraction()->onLeftMove(scenePos);
        }
        return false;
    }

    /// 交互模式鼠标左键释放事件
    case QEvent::MouseButtonRelease: {
        auto *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::LeftButton && activeInteraction()) {
            const QPointF scenePos = m_view->mapToScene(me->pos());
            activeInteraction()->onLeftRelease(scenePos);
            return true;
        }
        return false;
    }

    /// 交互模式按键按下事件
    case QEvent::KeyPress: {
        auto *ke = static_cast<QKeyEvent *>(event);
        if (activeInteraction())
            activeInteraction()->onKeyPress(ke);
        return false;
    }

    default:
        return QObject::eventFilter(obj, event);
    }
}
