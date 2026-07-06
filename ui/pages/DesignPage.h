#ifndef DESIGNPAGE_H
#define DESIGNPAGE_H

#include <QWidget>
#include <memory>

#include "core/model/GridModel.h"

class QTabBar;
class QSplitter;

class ComponentPanel;
class SimControlPanel;
class EditCanvas;
class InteractionManager;
class ModeButtonGroup;

/**
 * @class   DesignPage
 * @brief   设计页面，电路编辑主区域
 *
 * 组装四个子模块：
 *   - QTabBar 顶部标签栏（类似编辑器文件标签）
 *   - ComponentPanel 左侧元件选择
 *   - SimControlPanel 左侧仿真控制
 *   - EditCanvas 右侧电路画布
 */
class DesignPage : public QWidget
{
    Q_OBJECT

public:
    explicit DesignPage(QWidget *parent = nullptr);

private:
    void setupUI();
    void initGridModel();

    std::unique_ptr<GridModel> m_gridModel;

    QTabBar         *m_tabBar;
    QSplitter       *m_splitter;
    ComponentPanel  *m_componentPanel;
    SimControlPanel *m_simControlPanel;
    EditCanvas      *m_editCanvas;

    ModeButtonGroup     *m_modeGroup;
    InteractionManager  *m_interactionMgr;
};

#endif // DESIGNPAGE_H
