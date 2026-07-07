#include "ComponentRegistry.h"
#include "meta_component/Component.h"

// ═══════════════════════════════════════════════════════════
//  单例：静态局部变量，C++11 起线程安全
// ═══════════════════════════════════════════════════════════

ComponentRegistry& ComponentRegistry::instance()
{
    static ComponentRegistry reg;  // 首次调用时构造，程序结束时自动析构
    return reg;
}

// ═══════════════════════════════════════════════════════════
//  注册元件类型
// ═══════════════════════════════════════════════════════════
//  1. 分配 numericId（自增，确保唯一）
//  2. 填充 Entry（元数据 + 工厂函数）
//  3. 建立 id → 下标 / numericId → 下标 的双向索引
//  4. 按注册顺序维护去重的分类列表
// ═══════════════════════════════════════════════════════════

void ComponentRegistry::registerType(
    const QString &id, const QString &name,
    const QString &group, const ComponentSpec &spec,
    FactoryFunc factory)
{
    Entry entry;
    entry.meta.numericId  = m_nextNumericId++;  // ① 分配并递增
    entry.meta.id         = id;
    entry.meta.name       = name;
    entry.meta.group      = group;
    entry.meta.spec       = spec;
    entry.factory         = std::move(factory);  // ② 移动工厂函数，避免拷贝

    // ③ 索引：当前 entries.size() 就是新条目的下标（append 前）
    m_idToIndex[id]       = m_entries.size();
    m_numericIdToIndex[entry.meta.numericId] = m_entries.size();
    m_entries.append(std::move(entry));

    // ④ 维护分类列表（去重）：ComponentPanel 按此顺序显示
    if (!m_categoriesInOrder.contains(group)) {
        m_categoriesInOrder.append(group);
    }
}

// ═══════════════════════════════════════════════════════════
//  查询元数据
// ═══════════════════════════════════════════════════════════
//  通过 QMap 直接定位下标，O(1) 复杂度
//  ⚠️ 返回原始指针，调用方需确保 Registry 生命周期长于指针使用期
// ═══════════════════════════════════════════════════════════

const ComponentRegistry::ComponentMeta*
ComponentRegistry::find(const QString &id) const
{
    auto it = m_idToIndex.find(id);
    if (it == m_idToIndex.end())
        return nullptr;          // 未注册 → nullptr
    return &m_entries[it.value()].meta;
}

const ComponentRegistry::ComponentMeta*
ComponentRegistry::findByNumericId(int numericId) const
{
    auto it = m_numericIdToIndex.find(numericId);
    if (it == m_numericIdToIndex.end())
        return nullptr;
    return &m_entries[it.value()].meta;
}

// ═══════════════════════════════════════════════════════════
//  分类查询（供 ComponentPanel 填充树控件）
// ═══════════════════════════════════════════════════════════

/// 返回所有已注册的分类名（按首次注册的顺序）
QStringList ComponentRegistry::categories() const
{
    return m_categoriesInOrder;
}

/// 查询某个分类下的所有元件元数据
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
//  查找到对应的 Entry，调用其工厂函数。(x, y) 作为初始坐标传入。
//  工厂函数由 registerType 时注册，每种元件类型有自己的创建逻辑
//  （组装端口列表、创建 Behavior 子类等）
// ═══════════════════════════════════════════════════════════

std::unique_ptr<Component>
ComponentRegistry::create(const QString &id, int x, int y) const
{
    auto it = m_idToIndex.find(id);
    if (it == m_idToIndex.end())
        return nullptr;          // 未注册的类型
    auto comp = m_entries[it.value()].factory(x, y);
    if (comp)
        comp->setRegistryId(id);
    return comp;
}

std::unique_ptr<Component>
ComponentRegistry::createByNumericId(int numericId, int x, int y) const
{
    auto it = m_numericIdToIndex.find(numericId);
    if (it == m_numericIdToIndex.end())
        return nullptr;
    return m_entries[it.value()].factory(x, y);
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
