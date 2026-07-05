#ifndef MENUBAR_H
#define MENUBAR_H

#include <QObject>

class QMenuBar;

/**
 * @class   MenuBar
 * @brief   菜单栏，管理文件/编辑/视图/运行等菜单
 *
 * 接收 QMainWindow 的内置 QMenuBar 指针，
 * 在其上创建各菜单及动作项。
 */
class MenuBar : public QObject
{
    Q_OBJECT

public:
    explicit MenuBar(QMenuBar *menuBar, QObject *parent = nullptr);

private:
    QMenuBar *m_menuBar;
};

#endif // MENUBAR_H
