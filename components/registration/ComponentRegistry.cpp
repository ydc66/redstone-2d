#include "ComponentRegistry.h"
#include "core/meta_component/Component.h"

#include <QDebug>

// ═══════════════════════════════════════════════════════════
//  单例：静态局部变量，C++11 起线程安全
// ═══════════════════════════════════════════════════════════

ComponentRegistry& ComponentRegistry::instance()
{
    static ComponentRegistry reg;
    return reg;
}

// ═══════════════════════════════════════════════════════════
//  注册元件类型（私有，被模板方法调用）
// ═══════════════════════════════════════════════════════════

void ComponentRegistry::registerType(
    const QString &id, const QString &name,
    const QString &group, FactoryFunc factory)
{
    // 重复 ID 防护：元件 ID 是全局契约，冲突即程序员错误，拒绝注册并告警
    if (m_idToIndex.contains(id)) {
        qWarning() << "[ComponentRegistry] 重复注册元件 ID:" << id << "，已忽略";
        return;
    }

    Entry entry;
    entry.meta.id         = id;
    entry.meta.name       = name;
    entry.meta.group      = group;
    entry.factory         = std::move(factory);

    m_idToIndex[id]       = m_entries.size();
    m_entries.append(std::move(entry));

    if (!m_categoriesInOrder.contains(group)) {
        m_categoriesInOrder.append(group);
    }
}

// ═══════════════════════════════════════════════════════════
//  查询元数据
// ═══════════════════════════════════════════════════════════

const ComponentRegistry::ComponentMeta*
ComponentRegistry::find(const QString &id) const
{
    auto it = m_idToIndex.find(id);
    if (it == m_idToIndex.end())
        return nullptr;
    return &m_entries[it.value()].meta;
}

// ═══════════════════════════════════════════════════════════
//  分类查询（供 ComponentPanel 填充树控件）
// ═══════════════════════════════════════════════════════════

QStringList ComponentRegistry::categories() const
{
    return m_categoriesInOrder;
}

QList<const ComponentRegistry::ComponentMeta*>
ComponentRegistry::defsByCategory(const QString &cat) const
{
    QList<const ComponentMeta*> result;
    for (const auto &entry : m_entries) {
        if (entry.meta.group == cat)
            result.append(&entry.meta);
    }
    return result;
}

// ═══════════════════════════════════════════════════════════
//  创建元件实例
// ═══════════════════════════════════════════════════════════

std::unique_ptr<Component>
ComponentRegistry::create(const QString &id, int x, int y) const
{
    auto it = m_idToIndex.find(id);
    if (it == m_idToIndex.end())
        return nullptr;
    auto comp = m_entries[it.value()].factory(x, y);
    if (comp)
        comp->setRegistryId(id);
    return comp;
}

// ═══════════════════════════════════════════════════════════
//  列举所有 ID
// ═══════════════════════════════════════════════════════════

QStringList ComponentRegistry::allIds() const
{
    QStringList ids;
    for (const auto &entry : m_entries)
        ids.append(entry.meta.id);
    return ids;
}
