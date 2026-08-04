/**
 * @file   Interaction.h
 * @brief  交互模式策略基类 — 声明
 * @date   2026-07-05
 */

#pragma once

#include "InteractionMode.h"

#include <QObject>
#include <QCursor>
#include <QPointF>

class World;
class GridGraphicsScene;
class QGraphicsView;
class QKeyEvent;
class QWheelEvent;

/**
 * @class   Interaction
 * @brief   交互模式策略基类
 *
 * 每种操作模式实现为一个 Interaction 子类。
 * 虚函数默认空实现，子类只需覆写关心的。
 */
class Interaction : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief   构造函数
     * @param   world  世界（唯一编辑入口，UI 不直接碰 GridModel）
     * @param   scene  网格场景
     * @param   view   图形视图
     * @param   parent 父 QObject 对象
     */
    explicit Interaction(World *world, GridGraphicsScene *scene,
                         QGraphicsView *view, QObject *parent = nullptr);

    /** @brief 虚析构函数（默认） */
    virtual ~Interaction() = default;

    // ─── 鼠标事件（参数为场景坐标） ───
    /** @brief 左键按下 */
    virtual void onLeftPress(const QPointF &scenePos);
    /** @brief 左键移动（拖拽） */
    virtual void onLeftMove(const QPointF &scenePos);
    /** @brief 左键释放 */
    virtual void onLeftRelease(const QPointF &scenePos);
    /** @brief 右键按下（如旋转朝向） */
    virtual void onRightPress(const QPointF &scenePos);
    /** @brief 右键释放 */
    virtual void onRightRelease(const QPointF &scenePos);
    /** @brief 右键移动（拖拽） */
    virtual void onRightMove(const QPointF &scenePos);

    // ─── 通用鼠标移动（悬停/预览用） ───
    /** @brief 鼠标移动（悬停预览、虚影跟随等） */
    virtual void onMouseMove(const QPointF &scenePos);

    // ─── 键盘事件 ───
    /** @brief 键盘按键按下 */
    virtual void onKeyPress(QKeyEvent *event);

    // ─── 滚轮事件 ───
    /** @brief 滚轮滚动 */
    virtual void onWheel(QWheelEvent *event);

    // ─── 生命周期 ───
    /** @brief 切换到该模式时调用 */
    virtual void onActivate();
    /** @brief 切出该模式时调用 */
    virtual void onDeactivate();

    // ─── 光标 ───
    /** @brief 返回该模式下的光标样式 */
    virtual QCursor cursor() const;

protected:
    /**
     * @brief   将场景坐标转换为网格坐标
     * @param   scenePos 场景坐标
     * @return  对应的网格坐标（行列）
     */
    QPoint scenePosToGrid(const QPointF &scenePos) const;

    /** @brief 世界（编辑/查询统一入口） */
    World            *m_world;
    /** @brief 网格场景 */
    GridGraphicsScene *m_scene;
    /** @brief 图形视图 */
    QGraphicsView     *m_view;
};
