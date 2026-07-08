/**
 * @file   PlaceInteraction.cpp
 * @brief  放置模式交互实现
 * @date   2026-07-05
 */

#include "PlaceInteraction.h"

#include "components/registration/ComponentRegistry.h"
#include "core/model/GridModel.h"
#include "core/meta_component/Component.h"
#include "ui/pages/design_page/GridGraphicsScene.h"

#include <QGraphicsPixmapItem>
#include <QPainter>

/** @brief 析构时清理虚影图形项 */
PlaceInteraction::~PlaceInteraction()
{
    clearGhost();
}

/**
 * @brief   左键点击放置当前选中元件
 *
 * 若有效则清除虚影并发送 placeRequested 信号。
 */
void PlaceInteraction::onLeftPress(const QPointF &scenePos)
{
    if (m_activeComponentId.isEmpty())
        return;

    const QPoint grid = scenePosToGrid(scenePos);
    if (!canPlaceAt(grid.x(), grid.y()))
        return;

    clearGhost();
    emit placeRequested(grid.x(), grid.y(), m_activeComponentId);
}

/**
 * @brief   右键旋转当前元件朝向（顺时针 90°）
 *
 * 旋转后立即刷新虚影预览，确保连续右键有即时视觉反馈。
 */
void PlaceInteraction::onRightPress(const QPointF &scenePos)
{
    if (m_activeComponentId.isEmpty())
        return;

    // 顺时针旋转 90°
    m_ghostFacing = rotateRight(m_ghostFacing);
    m_ghostPixmap = QPixmap();   // 标记脏 → updateGhost 会重绘
    updateGhost(scenePos);       // 立即刷新虚影（不再等待鼠标移动）
}

/** @brief 鼠标移动时更新虚影预览位置 */
void PlaceInteraction::onMouseMove(const QPointF &scenePos)
{
    updateGhost(scenePos);
}

/** @brief 激活时标记虚影为脏（下次鼠标移动时刷新预览） */
void PlaceInteraction::onActivate()
{
    // 进入放置模式：如果已有 activeComponent，立刻跟随鼠标
    m_ghostPixmap = QPixmap();   // 标记脏
}

/** @brief 停用时清除虚影预览 */
void PlaceInteraction::onDeactivate()
{
    clearGhost();
}

/**
 * @brief   返回当前模式光标
 * @return  无待放置元件→ForbiddenCursor，有元件→CrossCursor
 */
QCursor PlaceInteraction::cursor() const
{
    if (m_activeComponentId.isEmpty())
        return Qt::ForbiddenCursor;
    return Qt::CrossCursor;
}

/**
 * @brief   检查指定网格位置是否可放置
 * @param   gx 网格列
 * @param   gy 网格行
 * @return  true 表示位置在边界内且为空
 */
bool PlaceInteraction::canPlaceAt(int gx, int gy) const
{
    if (!m_grid || !m_grid->isValid(gx, gy))
        return false;
    return !m_grid->cellAt(gx, gy);
}

/**
 * @brief   设置当前待放置的元件
 * @param   componentId 元件注册 ID
 *
 * 重置朝向北，标记虚影像图脏。
 */
void PlaceInteraction::setActiveComponent(const QString &componentId)
{
    m_activeComponentId = componentId;
    m_ghostFacing = Direction::North;
    m_ghostPixmap = QPixmap();   // 标记脏
}

// ─── 虚影预览 ─────────────────────────────────

/**
 * @brief   根据鼠标场景坐标更新虚影预览
 *
 * 组件 ID 或格子尺寸变化时触发重绘；
 * 若目标位置不可放置则隐藏虚影。
 */
void PlaceInteraction::updateGhost(const QPointF &scenePos)
{
    if (m_activeComponentId.isEmpty()) {
        clearGhost();
        return;
    }

    const int cs = m_scene ? m_scene->cellSize() : 64;

    // 组件 ID 或 格子尺寸 变化时重绘虚影
    if (m_ghostCachedId != m_activeComponentId
        || m_ghostCachedCellSize != cs
        || m_ghostPixmap.isNull())
    {
        renderGhostPixmap();
    }

    const QPoint grid = scenePosToGrid(scenePos);
    const bool canPlace = canPlaceAt(grid.x(), grid.y());

    if (!canPlace) {
        if (m_ghostItem)
            m_ghostItem->setVisible(false);
        return;
    }

    if (!m_ghostItem) {
        m_ghostItem = new QGraphicsPixmapItem();
        m_ghostItem->setZValue(998);   // 高亮(999)之下
        m_scene->addItem(m_ghostItem);
    }

    m_ghostItem->setPixmap(m_ghostPixmap);
    m_ghostItem->setPos(grid.x() * cs, grid.y() * cs);
    m_ghostItem->setVisible(true);
}

/** @brief 清除虚影：移除图形项并清空缓存 */
void PlaceInteraction::clearGhost()
{
    if (m_ghostItem) {
        m_scene->removeItem(m_ghostItem);
        delete m_ghostItem;
        m_ghostItem = nullptr;
    }
    m_ghostPixmap = QPixmap();
}

/**
 * @brief   渲染虚影像素图（半透明）
 *
 * 从 ComponentRegistry 创建临时实例，
 * 按当前朝向绘制并缓存到 m_ghostPixmap。
 */
void PlaceInteraction::renderGhostPixmap()
{
    const int cs = m_scene ? m_scene->cellSize() : 64;
    m_ghostPixmap = QPixmap(cs, cs);
    m_ghostPixmap.fill(Qt::transparent);

    if (!m_activeComponentId.isEmpty()) {
        auto comp = ComponentRegistry::instance().create(
            m_activeComponentId, 0, 0);
        if (comp) {
            comp->setFacing(m_ghostFacing);
            QPainter p(&m_ghostPixmap);
            p.setOpacity(0.45);
            comp->paint(&p, cs);
        }
    }

    m_ghostCachedId = m_activeComponentId;
    m_ghostCachedCellSize = cs;
}
