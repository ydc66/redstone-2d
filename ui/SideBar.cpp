#include "SideBar.h"

#include <QListWidget>
#include <QVBoxLayout>

/**
 * @brief   构造左侧导航侧边栏
 * @param   parent 父对象
 */
SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(80);// 设置固定宽度为 80 像素

    auto *layout = new QVBoxLayout(this);// 垂直盒子布局
    layout->setContentsMargins(0, 0, 0, 0);// 去掉布局四周的空白边距
    layout->setSpacing(0);// 去掉布局元素之间的间距

    m_listWidget = new QListWidget(this);
    m_listWidget->setSpacing(4);// 设置列表项之间的间距为 4 像素

    // 暂时不需要样式表
    m_listWidget->setStyleSheet(
        ""
    );

    // 添加列表项
    m_listWidget->addItem(QStringLiteral("欢迎"));
    m_listWidget->addItem(QStringLiteral("设计"));
    m_listWidget->addItem(QStringLiteral("帮助"));
    m_listWidget->addItem(QStringLiteral("设置"));

    // 默认选中第一项
    m_listWidget->setCurrentRow(0);

    layout->addWidget(m_listWidget);

    // 列表项选择信号连接到页面选择槽函数
    connect(m_listWidget, &QListWidget::currentRowChanged,
            this, &SideBar::pageSelected);
}
