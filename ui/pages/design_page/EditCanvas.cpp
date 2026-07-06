/**
 * @file   EditCanvas.cpp
 * @brief  电路编辑画布实现
 * @date   2026-07-05
 */

#include "EditCanvas.h"

#include "GridGraphicsScene.h"

#include "core/model/GridModel.h"

#include <QGraphicsView>
#include <QVBoxLayout>

EditCanvas::EditCanvas(GridModel *model, QWidget *parent)
    : QWidget(parent)
    , m_view(nullptr)
    , m_scene(nullptr)
{
    setupUI(model);
}

void EditCanvas::setupUI(GridModel *model)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    /// 创建网格场景
    m_scene = new GridGraphicsScene(model, this);

    /// 创建视图
    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing, true);// 开启抗锯齿
    m_view->setDragMode(QGraphicsView::NoDrag);// 禁用拖动模式
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);   // 设置变换锚点为鼠标位置
    m_view->setResizeAnchor(QGraphicsView::AnchorViewCenter);// 设置调整大小锚点为视图中心
    m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);// 设置视口更新模式为智能更新

    // 隐藏滚动条
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout->addWidget(m_view);

    // 延迟到显示后再居中（当 GridModel 设置后会更新 sceneRect）
    QMetaObject::invokeMethod(this, [this]() {
        m_view->centerOn(m_scene->sceneRect().center());
    }, Qt::QueuedConnection);
}
