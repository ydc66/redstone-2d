/**
 * @file   ComponentPanel.cpp
 * @brief  元件选择面板实现
 * @date   2026-07-05
 */

#include "ComponentPanel.h"

#include "components/registration/ComponentRegistry.h"

#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QFrame>

/**
 * @brief   构建设计页面
 * @param   parent 父对象
 */
ComponentPanel::ComponentPanel(QWidget *parent)
    : QWidget(parent)
    , m_tree(nullptr)
{
    setupUI();
}

void ComponentPanel::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    // ─── 分组标题 ───
    auto *title = new QLabel(QStringLiteral("🔌 元件选择"), this);
    title->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 13px;"));
    layout->addWidget(title);

    // ─── 分隔线 ───
    auto *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    // ─── 元件树 ───
    m_tree = new QTreeWidget(this);
    m_tree->setHeaderHidden(true);
    m_tree->setIndentation(16);
    m_tree->setAnimated(true);
    m_tree->setRootIsDecorated(true);

    // 从注册中心动态加载元件列表
    auto &registry = ComponentRegistry::instance();
    for (const auto &cat : registry.categories()) {
        auto *catItem = new QTreeWidgetItem(m_tree, {cat});
        catItem->setFlags(Qt::ItemIsEnabled);

        for (const auto *meta : registry.defsByCategory(cat)) {
            auto *item = new QTreeWidgetItem(catItem);
            item->setText(0, meta->name);
            item->setData(0, Qt::UserRole, meta->id);
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    }

    connect(m_tree, &QTreeWidget::itemClicked, this,
            [this](QTreeWidgetItem *item, int) {
        if (!item->parent())
            return;   // 跳过分类标题
        emit componentSelected(item->data(0, Qt::UserRole).toString());
    });

    layout->addWidget(m_tree, 1);
}
