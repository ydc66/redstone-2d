#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

class QListWidget;

/**
 * @class   SideBar
 * @brief   左侧导航侧边栏，切换右侧页面
 *
 * 包含欢迎、设计、帮助、设置四个导航项，
 * 选中时发射 pageSelected(int) 信号。
 */
class SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = nullptr);

signals:
    /**
     * @brief   页面选择信号
     * @param   index 选中的页面索引
     */
    void pageSelected(int index);

private:
    /**
     * @brief   列表
     */
    QListWidget *m_listWidget;
};

#endif // SIDEBAR_H
