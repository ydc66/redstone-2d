/**
 * @file   EditCanvas.cpp
 * @brief  电路编辑画布实现
 * @date   2026-07-05
 */

#include "EditCanvas.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>

/**
 * @brief   构建设计页面
 * @param   parent 父对象
 */
EditCanvas::EditCanvas(QWidget *parent)
    : QWidget(parent)
    , m_view(nullptr)
    , m_scene(nullptr)
{
    setupUI();
}

void EditCanvas::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    /// 创建场景
    m_scene = new QGraphicsScene(this);
    // 场景大小与视口一致（后续网格渲染时会按网格尺寸设置）
    m_scene->setBackgroundBrush(QColor(QStringLiteral("#1e1e1e")));

    /// 创建视图
    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing, true); /// 开启抗锯齿
    m_view->setDragMode(QGraphicsView::NoDrag); /// 禁用拖动模式
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse); /// 设置变换锚点为鼠标位置
    m_view->setResizeAnchor(QGraphicsView::AnchorViewCenter); /// 设置调整大小锚点为视图中心
    m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate); /// 设置视口更新模式为智能模式

    // 隐藏滚动条（后续通过缩放 + 中键平移导航）
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout->addWidget(m_view);
}
