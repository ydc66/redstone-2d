#include "DesignPage.h"

#include <QTabBar>
#include <QSplitter>
#include <QVBoxLayout>

#include "design_page/ComponentPanel.h"
#include "design_page/SimControlPanel.h"
#include "design_page/EditCanvas.h"
#include "design_page/GridGraphicsScene.h"
#include "design_page/ModeButtonGroup.h"
#include "design_page/InteractionManager.h"
#include "design_page/interaction_mode/PlaceInteraction.h"

#include "core/model/GridModel.h"

/**
 * @brief   构建设计页面
 * @param   parent 父对象
 */
DesignPage::DesignPage(QWidget *parent)
    : QWidget(parent)
    , m_tabBar(nullptr)
    , m_splitter(nullptr)
    , m_componentPanel(nullptr)
    , m_simControlPanel(nullptr)
    , m_editCanvas(nullptr)
    , m_modeGroup(nullptr)
    , m_interactionMgr(nullptr)
{
    // 先创建网格模型（画布初始化时需要）
    m_gridModel = std::make_unique<GridModel>();
    m_gridModel->resize(16, 16);

    setupUI();
    initGridModel();
}

void DesignPage::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ─── 顶部标签栏 ───
    // 类似编辑器的文件标签，支持动态增删、拖拽移动、关闭按钮
    m_tabBar = new QTabBar(this);
    m_tabBar->setExpanding(false);
    m_tabBar->setTabsClosable(true);
    m_tabBar->setMovable(true);
    m_tabBar->setElideMode(Qt::ElideRight);
    m_tabBar->setUsesScrollButtons(true);
    // 默认添加一个标签
    m_tabBar->addTab(QStringLiteral("电路 1"));
    mainLayout->addWidget(m_tabBar);

    // ─── 主体分栏 ───
    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->setHandleWidth(4);
    m_splitter->setChildrenCollapsible(false);

    // ─── 左侧面板（元件选择 + 仿真控制） ───
    auto *leftPanel = new QWidget(this);
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(8, 8, 8, 8);
    leftLayout->setSpacing(12);

    // ─── 操作模式切换（放在最顶部） ───
    m_modeGroup = new ModeButtonGroup(leftPanel);
    leftLayout->addWidget(m_modeGroup);

    m_componentPanel = new ComponentPanel(leftPanel);
    m_simControlPanel = new SimControlPanel(leftPanel);

    leftLayout->addWidget(m_componentPanel, 1);   // 元件面板占满剩余空间
    leftLayout->addWidget(m_simControlPanel);      // 仿真控制固定在底部

    leftPanel->setMinimumWidth(240);
    leftPanel->setMaximumWidth(350);
    m_splitter->addWidget(leftPanel);

    // ─── 右侧画布 ───
    m_editCanvas = new EditCanvas(m_gridModel.get(), this);
    m_splitter->addWidget(m_editCanvas);

    // QSplitter 默认比例：左侧 ~250px，右侧占满剩余
    m_splitter->setSizes({250, 800});

    mainLayout->addWidget(m_splitter, 1);
}

void DesignPage::initGridModel()
{
    // ─── 初始化交互管理器 ───
    if (auto *gridScene = m_editCanvas->scene()) {
        m_interactionMgr = new InteractionManager(
            m_editCanvas->view(), m_gridModel.get(), gridScene, this);
        m_interactionMgr->install();

        // ─── 元件面板选择 → 放置模式激活元件 ───
        connect(m_componentPanel, &ComponentPanel::componentSelected,
                m_interactionMgr->placeInteraction(),
                &PlaceInteraction::setActiveComponent);

        connect(m_modeGroup, &ModeButtonGroup::modeChanged,
                m_interactionMgr, &InteractionManager::setMode);
    }
}
