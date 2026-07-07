/**
 * @file   mainwindow.cpp
 * @brief  主窗口实现
 * @date   2026-07-05
 */

#include "mainwindow.h"

#include <QStatusBar>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>

#include "ui/SideBar.h"
#include "ui/MenuBar.h"
#include "ui/pages/WelcomePage.h"
#include "ui/pages/DesignPage.h"
#include "ui/pages/HelpPage.h"
#include "ui/pages/SettingsPage.h"

/**
 * @brief   构造主窗口
 * @param   parent 父对象
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ─── 窗口标题 ───
    setWindowTitle(QStringLiteral("Redstone 2D"));

    // ─── 默认大小并居中到屏幕 ───
    resize(1200, 800);
    // 获取主屏幕对象（主显示器），取它的可用区域（去除任务栏后的矩形），
    if (auto *screen = QApplication::primaryScreen()) {
        // 计算出窗口左上角坐标，使窗口居中显示（排除了任务栏等占位的矩形）
        auto geom = screen->availableGeometry();
        move((geom.width() - width()) / 2,
             (geom.height() - height()) / 2);
    }

    setupUI();
}

/**
 * @brief   组装界面
 *
 * 主体布局：水平 [SideBar | QStackedWidget]
 */
void MainWindow::setupUI()
{
    // ─── 菜单栏 ───
    // menuBar() 是 QMainWindow 自带的函数，返回内置菜单栏的指针
    m_menuBar = new MenuBar(menuBar(), this);

    // ─── 主体 ───
    auto *centralWidget = new QWidget(this);// 创建中心部件
    auto *mainLayout = new QHBoxLayout(centralWidget);// 水平盒子布局
    mainLayout->setContentsMargins(0, 0, 0, 0);// 去掉布局四周的空白边距
    mainLayout->setSpacing(0);// 去掉布局元素之间的间距，让 SideBar 和右侧页面紧挨着，没有缝隙

    // 左侧导航
    m_sideBar = new SideBar(this);
    mainLayout->addWidget(m_sideBar);// 添加左侧导航到布局

    // 右侧页面容器
    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(new WelcomePage(this));   // index 0
    m_stackedWidget->addWidget(new DesignPage(this));     // index 1
    m_stackedWidget->addWidget(new HelpPage(this));       // index 2
    m_stackedWidget->addWidget(new SettingsPage(this));   // index 3
    m_stackedWidget->setCurrentIndex(0);

    mainLayout->addWidget(m_stackedWidget, 1);// 添加右侧页面容器到布局，占满剩余空间
  
    setCentralWidget(centralWidget);// 设置中心部件

    // 左侧导航 → 右侧页面切换
    connect(m_sideBar, &SideBar::pageSelected,
            m_stackedWidget, &QStackedWidget::setCurrentIndex);
}


