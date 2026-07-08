/**
 * @file   SelectInteraction.h
 * @brief  选择/移动模式 — 声明
 * @date   2026-07-05
 */

#pragma once

#include "Interaction.h"

class QGraphicsRectItem;
class QGraphicsPixmapItem;
class QKeyEvent;

/**
 * @class   SelectInteraction
 * @brief   选择模式 — 点击选中，再次点击移动
 *
 * 流程：
 * 1. 点击元件 → 选中（高亮 + 显示虚影跟随鼠标）
 * 2. 鼠标移动 → 虚影跟随
 * 3. 点击空格子 → 将选中元件移动至目标格
 * 4. 点击其他元件 → 切换选中
 * 5. Delete/Backspace → 删除选中元件
 */
class SelectInteraction : public Interaction
{
    Q_OBJECT

public:
    using Interaction::Interaction;
    ~SelectInteraction() override;

    /** @brief 左键点击：选中 / 切换选中 / 移动元件 */
    void onLeftPress(const QPointF &scenePos) override;
    /** @brief 鼠标移动时更新移动虚影位置 */
    void onMouseMove(const QPointF &scenePos) override;
    /** @brief Delete/Backspace 删除选中元件 */
    void onKeyPress(QKeyEvent *event) override;
    /** @brief 激活时恢复移动虚影显示 */
    void onActivate() override;
    /** @brief 停用时清除选中状态与虚影 */
    void onDeactivate() override;
    /** @brief 返回箭头光标 */
    QCursor cursor() const override;

signals:
    /** @brief 选中元件变化时发出 */
    void componentSelected(const QString &id);
    /** @brief 元件移动时发出（移动到 -1,-1 表示删除） */
    void componentMoved(const QString &id, int newX, int newY);
    /** @brief 选中被清除时发出 */
    void selectionCleared();

private:
    /** @brief 清除选中状态、高亮和虚影 */
    void clearSelection();
    /** @brief 更新选中元件的高亮方框 */
    void updateHighlight();
    /** @brief 根据鼠标位置更新移动虚影 */
    void updateMoveGhost(const QPointF &scenePos);
    /** @brief 清除移动虚影（移除图形项并清空缓存） */
    void clearMoveGhost();
    /** @brief 渲染移动虚影像素图（半透明） */
    void renderMoveGhostPixmap();

    /** @brief 当前选中元件 ID（空表示无选中） */
    QString m_selectedId;
    /** @brief 选中元件当前所在网格坐标 */
    QPoint  m_selectedGrid;

    /** @brief 选中高亮方框图形项 */
    QGraphicsRectItem      *m_highlightRect   = nullptr;

    // ─── 移动虚影 ───
    /** @brief 移动虚影图形项 */
    QGraphicsPixmapItem    *m_moveGhostItem          = nullptr;
    /** @brief 缓存移动虚影像素图 */
    QPixmap                 m_moveGhostPixmap;
    /** @brief 缓存的元件 ID */
    QString                 m_moveGhostCachedId;
    /** @brief 缓存的格子尺寸 */
    int                     m_moveGhostCachedCellSize = 0;
};
