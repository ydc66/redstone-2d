#pragma once

#include "interaction_mode/InteractionMode.h"

#include <QWidget>

class QButtonGroup;
class QPushButton;

/**
 * @brief 侧边栏模式切换按钮组
 *
 * 位于左侧面板最顶部，按钮数量由 InteractionMode::Count 自动决定。
 */
class ModeButtonGroup : public QWidget
{
    Q_OBJECT

public:
    explicit ModeButtonGroup(QWidget *parent = nullptr);

    /// 设置当前模式
    void setMode(InteractionMode mode);
    /// 获取当前模式
    InteractionMode currentMode() const;

signals:
    /// 模式切换信号
    void modeChanged(InteractionMode mode);

private:
    void setupUI();
    /// 设置按钮组和按钮信号与槽的连接
    void setupConnections();
    /// 模式到按钮索引的转换
    int modeToIndex(InteractionMode mode) const;
    /// 按钮索引到模式的转换
    InteractionMode indexToMode(int index) const;

    /// 按钮组
    QButtonGroup *m_group;

    /// 按钮
    QPushButton  *m_buttons[static_cast<int>(InteractionMode::Count)];
};
