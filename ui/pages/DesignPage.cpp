#include "DesignPage.h"

#include <QLabel>
#include <QVBoxLayout>

/**
 * @brief   构建设计页面
 * @param   parent 父对象
 */
DesignPage::DesignPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *label = new QLabel(QStringLiteral("设计页面（电路编辑区域）"), this);
    label->setStyleSheet("font-size: 18px; color: #999;");

    layout->addWidget(label);
}
