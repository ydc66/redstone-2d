#pragma once

#include "InteractionMode.h"

#include <QObject>
#include <QCursor>
#include <QPointF>

class GridModel;
class GridGraphicsScene;
class QGraphicsView;
class QKeyEvent;

/**
 * @brief 交互模式策略基类
 *
 * 每种操作模式实现为一个 Interaction 子类。
 * 虚函数默认空实现，子类只需覆写关心的。
 */
class Interaction : public QObject
{
    Q_OBJECT

public:
    /// 构造函数 需要网格模型，场景，视图
    explicit Interaction(GridModel *grid, GridGraphicsScene *scene,
                         QGraphicsView *view, QObject *parent = nullptr);

    /// 析构函数 默认虚析构
    virtual ~Interaction() = default;

    // ─── 鼠标事件（参数为场景坐标） ───
    virtual void onLeftPress(const QPointF &scenePos);
    virtual void onLeftMove(const QPointF &scenePos);
    virtual void onLeftRelease(const QPointF &scenePos);

    // ─── 键盘事件 ───
    virtual void onKeyPress(QKeyEvent *event);

    // ─── 生命周期 ───
    virtual void onActivate();    /// 切换到该模式时调用
    virtual void onDeactivate();  /// 切出该模式时调用

    // ─── 光标 ───
    virtual QCursor cursor() const;

protected:
    /// 辅助：将场景坐标转换为网格坐标
    QPoint scenePosToGrid(const QPointF &scenePos) const;
    
    GridModel        *m_grid;
    GridGraphicsScene *m_scene;
    QGraphicsView     *m_view;
};
