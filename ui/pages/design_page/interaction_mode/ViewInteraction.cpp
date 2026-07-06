/**
 * @file   ViewInteraction.cpp
 * @brief  视图模式 — 左键平移画布，滚轮缩放画布
 *
 * 视图模式（ViewInteraction）
 * 提供画布的自由浏览功能，不修改任何电路数据。
 * 两种交互：
 *   1. 拖拽平移：通过修改视图变换矩阵实现
 *   2. 滚轮缩放：通过 QGraphicsView::scale() 实现
 */

#include "ViewInteraction.h"

#include <QGraphicsView>
#include <QWheelEvent>

//  左键按下 — 进入平移状态
void ViewInteraction::onLeftPress(const QPointF &scenePos)
{
    // ── 标记正在平移，用于 onLeftMove 判断是否继续 ──
    m_isPanning = true;

    // ── 记录按下的场景坐标，作为拖拽起点 ──
    //     后续 onLeftMove 通过当前坐标 − 上次坐标 计算偏移量
    m_lastPos = scenePos;

    // ── 切换手型为「抓握」状态，反馈给用户正在拖拽 ──
    m_view->setCursor(Qt::ClosedHandCursor);
}

//  鼠标移动 — 平移画布（仅在平移状态下有效）
void ViewInteraction::onLeftMove(const QPointF &scenePos)
{
    // ── 安全检查：未进入平移状态则忽略 ──
    if (!m_isPanning)
        return;

    // ── 计算本次移动相对于上一帧的偏移量（场景坐标） ──
    //     场景坐标是与缩放无关的绝对坐标，因此不受当前缩放倍率影响。
    const QPointF delta = scenePos - m_lastPos;
    m_lastPos = scenePos;

    // ── 通过修改视图变换矩阵实现平移 ──
    //     正确做法是前乘：Trans(−delta) * T。
    //     行向量展开：P * Trans(−delta) * T = (P − delta) * T。
    //     即先将场景偏移 −delta，再经缩放矩阵 T 映射到视口。
    QTransform offset;
    offset.translate(-delta.x(), -delta.y());
    m_view->setTransform(offset * m_view->transform());
}

//  左键释放 — 退出平移状态
void ViewInteraction::onLeftRelease(const QPointF & /*scenePos*/)
{
    // ── 清除平移标记 ──
    m_isPanning = false;

    // ── 恢复手型为「张开」，表示可拖拽 ──
    m_view->setCursor(Qt::OpenHandCursor);
}

//  滚轮滚动 — 等比缩放画布
void ViewInteraction::onWheel(QWheelEvent *event)
{
    // angleDelta().y() 表示垂直滚轮的滚动角度（单位：1/8 度）
    const double angle = event->angleDelta().y();

    // 缩放因子：向上 = 放大 1.15 倍，向下 = 缩小 1/1.15 ≈ 0.87 倍
    const double factor = (angle > 0) ? 1.15 : 1.0 / 1.15;

    // 对视图进行等比缩放
    m_view->scale(factor, factor);
}

//  光标样式 — 视图模式下始终显示张开的手型
QCursor ViewInteraction::cursor() const
{
    //  光标样式 — 视图模式下始终显示张开的手型
    return Qt::OpenHandCursor;
}
