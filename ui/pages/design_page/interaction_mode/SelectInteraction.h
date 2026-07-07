#pragma once

#include "Interaction.h"

class QGraphicsRectItem;
class QKeyEvent;

/**
 * @brief 选择模式 — 点击选中，拖拽移动
 */
class SelectInteraction : public Interaction
{
    Q_OBJECT

public:
    using Interaction::Interaction;

    void onLeftPress(const QPointF &scenePos) override;
    void onLeftMove(const QPointF &scenePos) override;
    void onLeftRelease(const QPointF &scenePos) override;
    void onKeyPress(QKeyEvent *event) override;
    void onActivate() override;
    void onDeactivate() override;
    QCursor cursor() const override;

signals:
    void componentSelected(const QString &id);
    void componentMoved(const QString &id, int newX, int newY);
    void selectionCleared();

private:
    void clearSelection();
    void updateHighlight();

    QString m_selectedId;
    bool    m_isDragging = false;
    QPoint  m_dragStartGrid;

    QGraphicsRectItem *m_highlightRect = nullptr;
};
