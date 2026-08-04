/**
 * @file   SelectInteraction.cpp
 * @brief  选择/移动模式实现
 * @date   2026-07-05
 */

#include "SelectInteraction.h"

#include "components/registration/ComponentRegistry.h"
#include "core/world/World.h"
#include "core/meta_component/Component.h"
#include "ui/pages/design_page/GridGraphicsScene.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>

/** @brief 析构时清理移动虚影 */
SelectInteraction::~SelectInteraction()
{
    clearMoveGhost();
}

/**
 * @brief   左键点击：选中 / 切换选中 / 移动元件
 *
 * 无选中时点击元件 → 选中并高亮；
 * 已有选中时点击另一元件 → 切换选中；
 * 已有选中时点击空格子 → 将元件移至该格。
 */
void SelectInteraction::onLeftPress(const QPointF &scenePos)
{
    const QPoint grid = scenePosToGrid(scenePos);
    if (!m_world || !m_world->isValid(grid.x(), grid.y()))
        return;

    auto *comp = m_world->cellAt(grid.x(), grid.y());

    if (m_selectedId.isEmpty()) {
        // ─── 无选中 → 尝试选中 ───
        if (!comp)
            return;
        m_selectedId = comp->registryId();
        m_selectedGrid = grid;
        updateHighlight();
        renderMoveGhostPixmap();
        updateMoveGhost(scenePos);
        emit componentSelected(m_selectedId);
    } else {
        // ─── 已有选中 ───
        if (comp) {
            // 点击另一个元件 → 切换选中
            m_selectedId = comp->registryId();
            m_selectedGrid = grid;
            updateHighlight();
            renderMoveGhostPixmap();
            updateMoveGhost(scenePos);
            emit componentSelected(m_selectedId);
        } else if (grid != m_selectedGrid) {
            // 点击空格子 → 移动（World 原子收口：remove + place + 回滚）
            if (m_world->moveComponent(
                    m_selectedGrid.x(), m_selectedGrid.y(),
                    grid.x(), grid.y())) {
                m_selectedGrid = grid;
                updateHighlight();
                renderMoveGhostPixmap();
                updateMoveGhost(scenePos);
                emit componentMoved(m_selectedId, grid.x(), grid.y());
            }
        } // 点击相同格子 → 无操作（保持选中）
    }
}

/** @brief 鼠标移动时更新移动虚影位置 */
void SelectInteraction::onMouseMove(const QPointF &scenePos)
{
    updateMoveGhost(scenePos);
}

/** @brief Delete/Backspace 删除选中元件 */
void SelectInteraction::onKeyPress(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        if (!m_selectedId.isEmpty()) {
            emit componentMoved(m_selectedId, -1, -1);
            clearSelection();
        }
    }
}

/** @brief 激活时恢复移动虚影显示 */
void SelectInteraction::onActivate()
{
    // 从其他模式切回时保持选中状态，恢复虚影
    if (!m_selectedId.isEmpty()) {
        renderMoveGhostPixmap();
    }
}

/** @brief 停用时清除选中状态与虚影 */
void SelectInteraction::onDeactivate()
{
    clearSelection();
}

/** @brief 返回箭头光标 */
QCursor SelectInteraction::cursor() const
{
    return Qt::ArrowCursor;
}

// ─── 选中操作 ─────────────────────────────────

/** @brief 清除选中状态、高亮和虚影，发送 selectionCleared 信号 */
void SelectInteraction::clearSelection()
{
    m_selectedId.clear();
    clearMoveGhost();
    if (m_highlightRect && m_scene) {
        m_scene->removeItem(m_highlightRect);
        delete m_highlightRect;
        m_highlightRect = nullptr;
    }
    emit selectionCleared();
}

/**
 * @brief   更新选中元件的高亮方框
 *
 * 移除旧方框，在选中格子位置绘制新的蓝色半透明方框。
 */
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

    const int cs = m_scene->cellSize();
    const int gx = m_selectedGrid.x();
    const int gy = m_selectedGrid.y();

    m_highlightRect = m_scene->addRect(gx * cs, gy * cs, cs, cs,
                                       QPen(QColor("#4A90D9"), 3),
                                       QColor(74, 144, 217, 50));
    m_highlightRect->setZValue(999);   // 保持在最上层
}

// ─── 移动虚影 ─────────────────────────────────

/**
 * @brief   根据鼠标位置更新移动虚影
 *
 * 若目标位置可移动则显示虚影，否则隐藏；
 * 组件 ID 或格子尺寸变化时触发重绘。
 */
void SelectInteraction::updateMoveGhost(const QPointF &scenePos)
{
    if (m_selectedId.isEmpty()) {
        clearMoveGhost();
        return;
    }

    const int cs = m_scene ? m_scene->cellSize() : 64;

    if (m_moveGhostCachedId != m_selectedId
        || m_moveGhostCachedCellSize != cs
        || m_moveGhostPixmap.isNull())
    {
        renderMoveGhostPixmap();
    }

    const QPoint grid = scenePosToGrid(scenePos);
    const bool canMove = (grid != m_selectedGrid)
                      && m_world && m_world->isValid(grid.x(), grid.y())
                      && !m_world->cellAt(grid.x(), grid.y());

    if (!canMove) {
        if (m_moveGhostItem)
            m_moveGhostItem->setVisible(false);
        return;
    }

    if (!m_moveGhostItem) {
        m_moveGhostItem = new QGraphicsPixmapItem();
        m_moveGhostItem->setZValue(997);   // 虚影在高亮之下
        m_scene->addItem(m_moveGhostItem);
    }

    m_moveGhostItem->setPixmap(m_moveGhostPixmap);
    m_moveGhostItem->setPos(grid.x() * cs, grid.y() * cs);
    m_moveGhostItem->setVisible(true);
}

/** @brief 清除移动虚影：移除图形项并清空缓存 */
void SelectInteraction::clearMoveGhost()
{
    if (m_moveGhostItem) {
        m_scene->removeItem(m_moveGhostItem);
        delete m_moveGhostItem;
        m_moveGhostItem = nullptr;
    }
    m_moveGhostPixmap = QPixmap();
}

/**
 * @brief   渲染移动虚影像素图（半透明）
 *
 * 从 ComponentRegistry 创建临时实例，
 * 使用已放置元件的实际朝向绘制并缓存。
 */
void SelectInteraction::renderMoveGhostPixmap()
{
    if (m_selectedId.isEmpty())
        return;

    const int cs = m_scene ? m_scene->cellSize() : 64;
    m_moveGhostPixmap = QPixmap(cs, cs);
    m_moveGhostPixmap.fill(Qt::transparent);

    // 获取实际元件的 facing（来自已放置的实例）
    Direction actualFacing = Direction::North;
    if (m_world && m_world->isValid(m_selectedGrid.x(), m_selectedGrid.y())) {
        if (auto *placed = m_world->cellAt(m_selectedGrid.x(), m_selectedGrid.y()))
            actualFacing = placed->facing();
    }

    auto comp = ComponentRegistry::instance().create(m_selectedId, 0, 0);
    if (comp) {
        comp->setFacing(actualFacing);
        QPainter p(&m_moveGhostPixmap);
        p.setOpacity(0.45);
        comp->paint(&p, cs);
    }

    m_moveGhostCachedId = m_selectedId;
    m_moveGhostCachedCellSize = cs;
}
