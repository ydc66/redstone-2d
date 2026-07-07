#pragma once

/**
 * @brief DesignPage 的操作模式
 *
 * 枚举值个数由 Count 自动维护。新增模式时在 Count 前加一项即可。
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
