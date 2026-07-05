#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SideBar;
class MenuBar;
class QStackedWidget;

/**
 * @class   MainWindow
 * @brief   主窗口
 *
 * 布局结构：
 *   - 顶部：MenuBar（文件、编辑、视图、运行）
 *   - 主体：SideBar（左侧导航）+ QStackedWidget（右侧页面）
 *   - 底部：StatusBar
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    void setupUI();

    SideBar         *m_sideBar;
    MenuBar         *m_menuBar;
    QStackedWidget  *m_stackedWidget;
};

#endif // MAINWINDOW_H
