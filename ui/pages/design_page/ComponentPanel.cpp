/**
 * @file   ComponentPanel.cpp
 * @brief  元件选择面板实现
 * @date   2026-07-05
 */

#include "ComponentPanel.h"

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

    // 添加四个分类（无子项，后续由注册机制注入）
    auto *catSignal   = new QTreeWidgetItem(m_tree, {QStringLiteral("信号源")});
    auto *catTrans    = new QTreeWidgetItem(m_tree, {QStringLiteral("传输元件")});
    auto *catMech     = new QTreeWidgetItem(m_tree, {QStringLiteral("机械元件")});
    auto *catBlock    = new QTreeWidgetItem(m_tree, {QStringLiteral("纯方块")});

    catSignal->setFlags(Qt::ItemIsEnabled);
    catTrans->setFlags(Qt::ItemIsEnabled);
    catMech->setFlags(Qt::ItemIsEnabled);
    catBlock->setFlags(Qt::ItemIsEnabled);

    layout->addWidget(m_tree, 1);
}
