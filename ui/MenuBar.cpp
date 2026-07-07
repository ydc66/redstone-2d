#include "MenuBar.h"

#include <QMenuBar>

/**
 * @brief   构造菜单栏
 * @param   menuBar QMainWindow 的内置菜单栏指针
 * @param   parent  父对象
 */
MenuBar::MenuBar(QMenuBar *menuBar, QObject *parent)
    : QObject(parent)
    , m_menuBar(menuBar)
{
    m_menuBar->addMenu(QStringLiteral("文件"));
    m_menuBar->addMenu(QStringLiteral("编辑"));
    m_menuBar->addMenu(QStringLiteral("视图"));
    m_menuBar->addMenu(QStringLiteral("运行"));
}
