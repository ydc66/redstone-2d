#include "SettingsPage.h"

#include <QLabel>
#include <QVBoxLayout>

/**
 * @brief   构建设置页面
 * @param   parent 父对象
 */
SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *label = new QLabel(QStringLiteral("设置页面"), this);
    label->setStyleSheet("font-size: 18px; color: #999;");

    layout->addWidget(label);
}
