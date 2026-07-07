#include "HelpPage.h"

#include <QLabel>
#include <QVBoxLayout>

/**
 * @brief   构造帮助页面
 * @param   parent 父对象
 */
HelpPage::HelpPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *label = new QLabel(QStringLiteral("帮助页面"), this);
    label->setStyleSheet("font-size: 18px; color: #999;");

    layout->addWidget(label);
}
