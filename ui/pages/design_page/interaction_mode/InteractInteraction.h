#pragma once

#include "Interaction.h"

/**
 * @brief 交互模式 — 点击可交互元件触发行为
 */
class InteractInteraction : public Interaction
{
    Q_OBJECT

public:
    using Interaction::Interaction;

    void onLeftPress(const QPointF &scenePos) override;
    QCursor cursor() const override;

signals:
    void interactTriggered(const QString &componentId, int gridX, int gridY);
};
