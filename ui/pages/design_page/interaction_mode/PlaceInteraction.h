/**
 * @file   PlaceInteraction.h
 * @brief  放置模式交互 — 声明
 * @date   2026-07-05
 */

#pragma once

#include "Interaction.h"

#include "core/meta_component/Direction.h"

class QGraphicsPixmapItem;

/**
 * @class   PlaceInteraction
 * @brief   放置模式 — 点击网格放置元件
 *
 * 特性：
 * - 鼠标悬停可放置区域时显示半透明虚影预览
 * - 右键旋转当前元件朝向
 * - 左键点击将元件以当前朝向放入网格
 */
class PlaceInteraction : public Interaction
{
    Q_OBJECT

public:
    using Interaction::Interaction;
    ~PlaceInteraction() override;

    /** @brief 左键点击放置当前选中元件 */
    void onLeftPress(const QPointF &scenePos) override;
    /** @brief 右键旋转当前元件朝向（顺时针 90°） */
    void onRightPress(const QPointF &scenePos) override;
    /** @brief 鼠标移动更新虚影预览位置 */
    void onMouseMove(const QPointF &scenePos) override;
    /** @brief 激活时标记虚影为脏（下次移动时刷新） */
    void onActivate() override;
    /** @brief 停用时清除虚影 */
    void onDeactivate() override;
    /** @brief 返回对应光标（无元件→禁止，有元件→十字） */
    QCursor cursor() const override;

    /**
     * @brief   检查指定网格位置是否可放置
     * @param   gx 网格列
     * @param   gy 网格行
     * @return  true 表示空格子且在边界内
     */
    bool canPlaceAt(int gx, int gy) const;

    /** @brief 返回当前预览朝向（创建元件时需使用此朝向） */
    Direction currentFacing() const { return m_ghostFacing; }

public slots:
    /**
     * @brief   设置当前待放置的元件
     * @param   componentId 元件注册 ID
     */
    void setActiveComponent(const QString &componentId);

signals:
    /**
     * @brief   请求在指定网格位置放置元件
     * @param   gridX       目标网格列
     * @param   gridY       目标网格行
     * @param   componentId 待放置元件的注册 ID
     */
    void placeRequested(int gridX, int gridY, const QString &componentId);

private:
    /** @brief 根据鼠标位置更新虚影预览 */
    void updateGhost(const QPointF &scenePos);
    /** @brief 清除虚影（移除图形项并清空缓存） */
    void clearGhost();
    /** @brief 渲染虚影像素图（带透明度） */
    void renderGhostPixmap();

    /** @brief 当前选中的待放置元件 ID */
    QString m_activeComponentId;

    // ─── 虚影预览 ───
    /** @brief 当前预览朝向 */
    Direction                m_ghostFacing   = Direction::North;
    /** @brief 虚影预览图形项 */
    QGraphicsPixmapItem     *m_ghostItem     = nullptr;
    /** @brief 缓存虚影像素图 */
    QPixmap                  m_ghostPixmap;
    /** @brief 缓存的元件 ID（用于判断是否需要重新渲染） */
    QString                  m_ghostCachedId;
    /** @brief 缓存的格子尺寸 */
    int                      m_ghostCachedCellSize = 0;
};
