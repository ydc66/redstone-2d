#include "SelectInteraction.h"

#include "core/model/GridModel.h"
#include "core/meta_component/Component.h"
#include "ui/pages/design_page/GridGraphicsScene.h"

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QPen>

#include <QString>

void SelectInteraction::onLeftPress(const QPointF &scenePos)
{
    const QPoint grid = scenePosToGrid(scenePos);
    if (!m_grid || !m_grid->isValid(grid.x(), grid.y()))
        return;

    auto *comp = m_grid->cellAt(grid.x(), grid.y());
    if (!comp) {
        clearSelection();
        return;
    }

    m_selectedId = comp->registryId();
    m_isDragging = true;
    m_dragStartGrid = grid;
    updateHighlight();
    emit componentSelected(m_selectedId);
}

void SelectInteraction::onLeftMove(const QPointF &scenePos)
{
    if (!m_isDragging || m_selectedId.isEmpty())
        return;

    const QPoint grid = scenePosToGrid(scenePos);
    if (grid != m_dragStartGrid && m_grid && m_grid->isValid(grid.x(), grid.y())) {
        // 仅在目标格为空时移动元件
        if (!m_grid->cellAt(grid.x(), grid.y())) {
            auto comp = m_grid->removeComponentAt(m_dragStartGrid.x(), m_dragStartGrid.y());
            if (comp) {
                m_grid->placeComponent(grid.x(), grid.y(), std::move(comp));
                m_scene->update();
            }
        }
        m_dragStartGrid = grid;
        updateHighlight();
        emit componentMoved(m_selectedId, grid.x(), grid.y());
    }
}

void SelectInteraction::onLeftRelease(const QPointF & /*scenePos*/)
{
    m_isDragging = false;
}

void SelectInteraction::onKeyPress(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        if (!m_selectedId.isEmpty()) {
            emit componentMoved(m_selectedId, -1, -1);
            clearSelection();
        }
    }
}

void SelectInteraction::onActivate()
{
    // 进入选择模式不清除已有选中（从其他模式切回时保持选中状态）
}

void SelectInteraction::onDeactivate()
{
    clearSelection();
}

QCursor SelectInteraction::cursor() const
{
    return Qt::ArrowCursor;
}

void SelectInteraction::clearSelection()
{
    m_selectedId.clear();
    m_isDragging = false;
    if (m_highlightRect && m_scene) {
        m_scene->removeItem(m_highlightRect);
        delete m_highlightRect;
        m_highlightRect = nullptr;
    }
    emit selectionCleared();
}

void SelectInteraction::updateHighlight()
{
    if (!m_scene) return;

    // 移除旧高亮
    if (m_highlightRect) {
        m_scene->removeItem(m_highlightRect);
        delete m_highlightRect;
        m_highlightRect = nullptr;
    }

    if (m_selectedId.isEmpty())
        return;

    // 从 registryId 恢复坐标（这里 m_dragStartGrid 保存了当前选中坐标）
    const int gx = m_dragStartGrid.x();
    const int gy = m_dragStartGrid.y();

    const int cs = m_scene->cellSize();

    m_highlightRect = m_scene->addRect(gx * cs, gy * cs, cs, cs,
                                       QPen(QColor("#4A90D9"), 3),
                                       QColor(74, 144, 217, 50));
    m_highlightRect->setZValue(999);   // 保持在最上层
}
