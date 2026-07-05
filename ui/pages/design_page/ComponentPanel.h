#ifndef COMPONENTPANEL_H
#define COMPONENTPANEL_H

#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

/**
 * @class   ComponentPanel
 * @brief   元件选择面板
 *
 * 左侧面板的元件选择区域，使用 QTreeWidget 展示四类元件。
 * 当前骨架阶段只展示分类名称，具体子项由后续注册机制自动注入。
 */
class ComponentPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentPanel(QWidget *parent = nullptr);

signals:
    /**
     * @brief   元件选择信号
     * @param   componentId 元件 ID
     */
    void componentSelected(const QString &componentId);

private:
    void setupUI();

    QTreeWidget *m_tree;
};

#endif // COMPONENTPANEL_H
