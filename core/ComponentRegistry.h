#pragma once

#include "types/ComponentSpec.h"

#include <QList>
#include <QMap>
#include <QString>
#include <memory>

class Component;

/**
 * @brief 元件注册中心 — 单例工厂
 *
 * 职责：
 *   1. 管理所有元件类型的注册表（元数据 + 工厂函数）
 *   2. 提供按字符串 ID（如 "solid_block"）或 numericId 的查询
 *   3. 通过工厂函数创建元件实例
 *
 * 数据流：
 *   registerType()  → 注册元件类型
 *         ↓
 *   create(id)      → 调用对应的工厂函数，返回 unique_ptr<Component>
 *   find(id)        → 查询元数据（名称、分组、物理属性等）
 *
 * 索引策略：
 *   使用 QMap 建立 id → 数组下标 和 numericId → 数组下标的映射，
 *   实际数据集中在 QList<Entry> 中，避免多份拷贝。
 */
class ComponentRegistry
{
public:
    /// 获取单例
    static ComponentRegistry& instance();

    /**
     * @brief 元件元数据——注册时确定的静态信息
     */
    struct ComponentMeta
    {
        int           numericId = -1;   ///< 整数 ID，注册时自动递增（适合网络/存档序列化）
        QString       id;               ///< 字符串 ID，如 "solid_block"（适合代码中引用）
        QString       name;             ///< 显示名称，如 "实心方块"（UI 展示用）
        QString       group;            ///< UI 分组名，对应 ComponentPanel 中的分类（"信号源" / "传输元件" / "机械元件" / "纯方块"）
        ComponentSpec spec;             ///< 物理属性（Category、可推动性、基础信号强度）
    };

    /**
     * @brief 工厂函数签名
     * @param x, y  创建时赋予的网格坐标
     * @return      元件的 unique_ptr
     */
    using FactoryFunc = std::function<std::unique_ptr<Component>(int, int)>;

    // ─── 注册 ───
    void registerType(const QString &id, const QString &name,
                      const QString &group, const ComponentSpec &spec,
                      FactoryFunc factory);

    // ─── 查询元数据（不创建实例） ───
    const ComponentMeta* find(const QString &id) const;
    const ComponentMeta* findByNumericId(int numericId) const;

    // ─── 分类查询（ComponentPanel 填充用） ───
    QStringList categories() const;                                  // 所有分类名（按注册顺序）
    QList<const ComponentMeta*> defsByCategory(const QString &cat) const;  // 某分类下的所有元件

    // ─── 工厂创建 ───
    std::unique_ptr<Component> create(const QString &id, int x, int y) const;
    std::unique_ptr<Component> createByNumericId(int numericId, int x, int y) const;

    // ─── 列举 ───
    QStringList allIds() const;

private:
    ComponentRegistry() = default;

    /// 内部条目：元数据 + 工厂函数
    struct Entry
    {
        ComponentMeta meta;
        FactoryFunc   factory;
    };

    QList<Entry>          m_entries;              ///< 主数据区，下标 = 注册顺序
    QMap<QString, int>    m_idToIndex;            ///< 字符串 ID → entries[] 下标
    QMap<int, int>        m_numericIdToIndex;     ///< numericId → entries[] 下标
    QStringList           m_categoriesInOrder;    ///< 分类列表（保持注册顺序，去重）
    int                   m_nextNumericId = 0;    ///< 下一个可用的 numericId，每次注册后 +1
};
