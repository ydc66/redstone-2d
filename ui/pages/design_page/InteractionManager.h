#pragma once

#include "interaction_mode/InteractionMode.h"
#include "interaction_mode/Interaction.h"

#include <QObject>

class GridModel;
class GridGraphicsScene;
class QGraphicsView;
class QEvent;

/**
 * @brief 交互事件分发核心
 *
 * 在 QGraphicsView 上安装事件过滤器，根据当前模式将事件委托给对应的
 * Interaction 子类处理。右键菜单作为全局操作直接处理。
 */
class InteractionManager : public QObject
{
    Q_OBJECT

public:
    explicit InteractionManager(QGraphicsView *view,
                                 GridModel *grid,
                                 GridGraphicsScene *scene,
                                 QObject *parent = nullptr);

    /// 安装/移除事件过滤器
    void install();
    void remove();

    /// 获取当前模式的 Interaction 实例
    Interaction *activeInteraction() const { return m_interactions[static_cast<int>(m_currentMode)]; }

    /// 当前模式
    InteractionMode currentMode() const { return m_currentMode; }

public slots:
    /// 切换模式
    void setMode(InteractionMode mode);

signals:
    /// 模式切换信号
    void modeChanged(InteractionMode mode);

protected:
    /// 事件过滤器
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QGraphicsView     *m_view;
    GridModel         *m_grid;
    GridGraphicsScene *m_scene;
    
    /// 当前模式
    InteractionMode    m_currentMode = InteractionMode::View;
    // 按 InteractionMode::Count 自动适配模式数量
    Interaction *m_interactions[static_cast<int>(InteractionMode::Count)] = {};
};
