#pragma once

#include "Interaction.h"

/**
 * @brief 视图模式 — 左键平移画布，滚轮缩放画布
 */
class ViewInteraction : public Interaction
{
    Q_OBJECT

public:
    using Interaction::Interaction;

    void onLeftPress(const QPointF &scenePos) override;
    void onLeftMove(const QPointF &scenePos) override;
    void onLeftRelease(const QPointF &scenePos) override;

    void onWheel(QWheelEvent *event) override;

    QCursor cursor() const override;

private:
    bool    m_isPanning = false;
    QPointF m_lastPos;
};
