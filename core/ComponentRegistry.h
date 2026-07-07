#pragma once

#include <QList>
#include <QMap>
#include <QString>
#include <memory>
#include <functional>

class Component;

/**
 * @brief 元件注册中心 — 单例工厂
 *
 * 职责：
 *   1. 管理所有元件类型的注册表（元数据 + 工厂函数）
 *   2. 提供按字符串 ID（如 "solid_block"）或 numericId 的查询
 *   3. 通过模板方法 registerType<T>() 自动生成工厂函数
 *
 * 数据流：
 *   registerType<T>(id, name, group)  → 注册元件类型
 *         ↓
 *   create(id)      → 调用对应的工厂函数，返回 unique_ptr<Component>
 *   find(id)        → 查询元数据（名称、分组）
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
        QString       group;            ///< UI 分组名，对应 ComponentPanel 中的分类
    };

    /**
     * @brief 工厂函数签名
     * @param x, y  创建时赋予的网格坐标
     * @return      元件的 unique_ptr
     */
    using FactoryFunc = std::function<std::unique_ptr<Component>(int, int)>;

    // ─── 模板注册（自动生成工厂 lambda） ───
    template<typename T>
    void registerType(const QString &id, const QString &name,
                      const QString &group) {
        registerType(id, name, group, [](int x, int y) {
            return std::make_unique<T>(x, y);
        });
    }

    // ─── 查询元数据（不创建实例） ───
    const ComponentMeta* find(const QString &id) const;
    const ComponentMeta* findByNumericId(int numericId) const;

    // ─── 分类查询（ComponentPanel 填充用） ───
    QStringList categories() const;
    QList<const ComponentMeta*> defsByCategory(const QString &cat) const;

    // ─── 工厂创建 ───
    std::unique_ptr<Component> create(const QString &id, int x, int y) const;
    std::unique_ptr<Component> createByNumericId(int numericId, int x, int y) const;

    // ─── 列举 ───
    QStringList allIds() const;

private:
    ComponentRegistry() = default;

    /// 内部注册（接受显式工厂）
    void registerType(const QString &id, const QString &name,
                      const QString &group, FactoryFunc factory);

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
