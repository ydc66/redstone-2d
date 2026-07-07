#pragma once

#include "Interaction.h"

/**
 * @brief 放置模式 — 点击网格放置元件
 */
class PlaceInteraction : public Interaction
{
    Q_OBJECT

public:
    using Interaction::Interaction;

    void onLeftPress(const QPointF &scenePos) override;
    QCursor cursor() const override;

    /**
     * @brief 判断是否可以在指定网格位置放置元件
     * @param gx 网格X坐标
     * @param gy 网格Y坐标
     * @return true 可以放置
     * @return false 不能放置
     */
    bool canPlaceAt(int gx, int gy) const;

public slots:
    /**
     * @brief 设置当前选中的元件ID
     * @param componentId 元件ID
     */
    void setActiveComponent(const QString &componentId);

signals:
    /**
     * @brief 请求放置元件
     * @param gridX 网格X坐标
     * @param gridY 网格Y坐标
     * @param componentId 元件ID
     */
    void placeRequested(int gridX, int gridY, const QString &componentId);

private:
    /**
     * @brief 当前选中的元件ID
     */
    QString m_activeComponentId;
};
