/**
 * @file   ModeButtonGroup.cpp
 * @brief  侧边栏模式切换按钮组实现
 */

#include "ModeButtonGroup.h"

#include <QButtonGroup>
#include <QPushButton>
#include <QHBoxLayout>

namespace {
constexpr int MODE_COUNT = static_cast<int>(InteractionMode::Count);
}

ModeButtonGroup::ModeButtonGroup(QWidget *parent)
    : QWidget(parent)
    , m_group(nullptr)
{
    setupUI();
    setupConnections();
    setMode(InteractionMode::View);
}

void ModeButtonGroup::setMode(InteractionMode mode)
{
    const int idx = modeToIndex(mode);
    if (idx >= 0 && idx < MODE_COUNT && m_buttons[idx])
        m_buttons[idx]->setChecked(true); // 设置当前模式按钮为选中状态
}

InteractionMode ModeButtonGroup::currentMode() const
{
    if (!m_group)
        return InteractionMode::View;

    // 获取当前选中的按钮
    auto *btn = m_group->checkedButton();
    if (!btn)
        return InteractionMode::View;

    for (int i = 0; i < MODE_COUNT; ++i) {
        if (m_buttons[i] == btn)
            return indexToMode(i);
    }
    return InteractionMode::View;
}

void ModeButtonGroup::setupUI()
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    // 创建按钮组
    m_group = new QButtonGroup(this);
    m_group->setExclusive(true); // 设置按钮组为单选模式

    const char *labels[MODE_COUNT] = {"视图", "选择", "放置", "交互", "破坏"}; // 模式按钮标签
    const char *tips[MODE_COUNT]   = {"视图", "选择", "放置", "交互", "破坏"};   // 模式按钮提示

    // 循环创建模式按钮
    for (int i = 0; i < MODE_COUNT; ++i) {
        // 创建按钮
        m_buttons[i] = new QPushButton(QString::fromUtf8(labels[i]), this);
        m_buttons[i]->setCheckable(true);
        m_buttons[i]->setToolTip(QString::fromUtf8(tips[i]));
        m_buttons[i]->setFixedSize(40, 28); // 设置按钮大小
        m_buttons[i]->setCursor(Qt::PointingHandCursor); // 设置按钮鼠标悬停时的光标
        // 将按钮添加到按钮组
        m_group->addButton(m_buttons[i], i);
        // 将按钮添加到布局中
        layout->addWidget(m_buttons[i]);
    }

    layout->addStretch();
}

void ModeButtonGroup::setupConnections()
{
    connect(m_group, &QButtonGroup::idClicked, this, [this](int id) {
        emit modeChanged(indexToMode(id));
    });
}

int ModeButtonGroup::modeToIndex(InteractionMode mode) const
{
    switch (mode) {
    case InteractionMode::View:     return 0;
    case InteractionMode::Select:   return 1;
    case InteractionMode::Place:    return 2;
    case InteractionMode::Interact: return 3;
    case InteractionMode::Destroy:  return 4;
    default:                        return 0;
    }
}

InteractionMode ModeButtonGroup::indexToMode(int index) const
{
    switch (index) {
    case 0:  return InteractionMode::View;
    case 1:  return InteractionMode::Select;
    case 2:  return InteractionMode::Place;
    case 3:  return InteractionMode::Interact;
    case 4:  return InteractionMode::Destroy;
    default: return InteractionMode::View;
    }
}
