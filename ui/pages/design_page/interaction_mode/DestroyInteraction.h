#pragma once

#include "Interaction.h"

/**
 * @brief 破坏模式 — 点击移除元件
 */
class DestroyInteraction : public Interaction
{
    Q_OBJECT

public:
    using Interaction::Interaction;

    void onLeftPress(const QPointF &scenePos) override;
    QCursor cursor() const override;

signals:
    void componentDestroyed(const QString &componentId, int gridX, int gridY);
};
