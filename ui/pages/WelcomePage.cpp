#include "WelcomePage.h"

#include <QLabel>
#include <QVBoxLayout>

/**
 * @brief   构造欢迎页面
 * @param   parent 父对象
 */
WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *title = new QLabel(QStringLiteral("欢迎使用 Redstone 2D"), this);
    title->setStyleSheet("font-size: 28px; font-weight: bold; color: #333;");

    auto *desc = new QLabel(QStringLiteral("红石电路模拟器"), this);
    desc->setStyleSheet("font-size: 16px; color: #777;");

    layout->addWidget(title);
    layout->addWidget(desc);
}
