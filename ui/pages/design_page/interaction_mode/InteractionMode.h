#pragma once

/**
 * @brief DesignPage 的 5 种操作模式
 *
 * 通过 ModeButtonGroup 切换，由 InteractionManager 根据当前模式路由事件。
 */
enum class InteractionMode
{
    View,       /// 视图：拖拽平移画布 + 滚轮缩放
    Select,     /// 选择：选中/拖拽元件
    Place,      /// 放置：点击网格放置元件
    Interact,   /// 交互：点击可交互元件触发行为
    Destroy,     /// 破坏：点击移除元件
    Count        /// 模式数量
};
