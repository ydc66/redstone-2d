/**
 * @file   SimControlPanel.cpp
 * @brief  仿真控制面板实现
 * @date   2026-07-05
 */

#include "SimControlPanel.h"

#include <cmath>

#include <QStringList>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

// ─── 速度档位常量 ───

static constexpr int kSpeedPresetCount = 3;

static const QStringList kSpeedPresetLabels = {
    QStringLiteral("0.5×"),
    QStringLiteral("1×"),
    QStringLiteral("2×"),
};

/// 预设速度倍率值（与标签一一对应）
static constexpr double kSpeedPresetValues[kSpeedPresetCount] = {
    0.5, 1.0, 2.0
};

// ─── 滑块值 ↔ 速度倍率 转换 ───
// 对数映射：0 ← 0.2×，50 ← 1×，100 ← 5×
// speed = 0.2 * 25^(value/100)

// 滑块值 → 速度倍率
static double sliderToSpeed(int sliderValue)
{
    double t = sliderValue / 100.0;
    return 0.2 * std::pow(25.0, t);
}

// 速度倍率 → 滑块值
static int speedToSlider(double speed)
{
    double t = std::log(speed / 0.2) / std::log(25.0);
    return static_cast<int>(std::round(t * 100.0));
}

// 速度倍率 → 格式化字符串
static QString formatSpeed(double speed)
{
    return QString::number(speed, 'f', 2) + QStringLiteral("×");
}

/// 查找最近的预设档位索引（距离 ≤2 时匹配），否则返回 -1
static int findNearestPreset(int sliderValue)
{
    for (int i = 0; i < kSpeedPresetCount; ++i) {
        int ps = speedToSlider(kSpeedPresetValues[i]);
        if (std::abs(sliderValue - ps) <= 2)
            return i;
    }
    return -1;
}

// ═══════════════════════════════════════════════════════════════════

SimControlPanel::SimControlPanel(QWidget *parent)
    : QWidget(parent)
    , m_btnRun(nullptr)
    , m_btnStep(nullptr)
    , m_btnPreset{}
    , m_btnCustom(nullptr)
    , m_speedGroup(nullptr)
    , m_speedSlider(nullptr)
    , m_speedLabel(nullptr)
{
    setupUI();
    setupConnections();
}

void SimControlPanel::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    // ─── 分组标题 ───
    auto *title = new QLabel(QStringLiteral("⚙ 仿真控制"), this);
    title->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 13px;"));
    layout->addWidget(title);

    // ─── 分隔线 ───
    auto *sep1 = new QFrame(this);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    layout->addWidget(sep1);

    // ─── 运行 / 步进按钮 ───
    auto *btnRow = new QHBoxLayout;
    btnRow->setSpacing(6);

    m_btnRun = new QPushButton(QStringLiteral("▶ 运行"), this);
    m_btnStep = new QPushButton(QStringLiteral("⏭ 步进"), this);
    btnRow->addWidget(m_btnRun);
    btnRow->addWidget(m_btnStep);
    layout->addLayout(btnRow);

    // ─── 分隔线 ───
    auto *sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setFrameShadow(QFrame::Sunken);
    layout->addWidget(sep2);

    // ─── 运行速度 ───
    auto *speedLabel = new QLabel(QStringLiteral("运行速度"), this);
    speedLabel->setStyleSheet(QStringLiteral("font-size: 12px;"));
    layout->addWidget(speedLabel);

    // ─── 速度预设按钮组（互斥选中） ───
    m_speedGroup = new QButtonGroup(this);
    m_speedGroup->setExclusive(true);

    auto *speedBtnRow = new QHBoxLayout;
    speedBtnRow->setSpacing(4);

    for (int i = 0; i < kPresetCount; ++i) {
        m_btnPreset[i] = new QPushButton(kSpeedPresetLabels[i], this);
        m_btnPreset[i]->setCheckable(true);
        m_speedGroup->addButton(m_btnPreset[i], i);
        speedBtnRow->addWidget(m_btnPreset[i]);
    }

    // 自定义按钮
    m_btnCustom = new QPushButton(QStringLiteral("自定义"), this);
    m_btnCustom->setCheckable(true);
    m_speedGroup->addButton(m_btnCustom, kCustomId);
    speedBtnRow->addWidget(m_btnCustom);

    layout->addLayout(speedBtnRow);

    // ─── 速度滑块 + 倍率显示 ───
    auto *sliderRow = new QHBoxLayout;
    sliderRow->setSpacing(6);

    m_speedSlider = new QSlider(Qt::Horizontal, this);
    m_speedSlider->setRange(kSliderMin, kSliderMax);
    m_speedSlider->setValue(kSliderMax / 2);       // 初始 1×
    m_speedSlider->setPageStep(5);
    sliderRow->addWidget(m_speedSlider, 1);         // stretch = 1

    m_speedLabel = new QLabel(QStringLiteral("1.00×"), this);
    m_speedLabel->setFixedWidth(50);
    m_speedLabel->setAlignment(Qt::AlignCenter);
    m_speedLabel->setStyleSheet(QStringLiteral("font-size: 12px;"));
    sliderRow->addWidget(m_speedLabel);

    layout->addLayout(sliderRow);

    // 默认选中 1×
    m_btnPreset[1]->setChecked(true);
}

void SimControlPanel::setupConnections()
{
    // 速度按钮点击 → 同步滑块
    connect(m_speedGroup, &QButtonGroup::idClicked,
            this, &SimControlPanel::onSpeedButtonClicked);

    // 滑块值变化 → 同步按钮与显示
    connect(m_speedSlider, &QSlider::valueChanged,
            this, &SimControlPanel::onSliderMoved);
}

void SimControlPanel::onSpeedButtonClicked(int id)
{
    // 自定义按钮不做调整，保持当前滑块位置
    if (id < 0 || id >= kPresetCount)
        return;

    double speed = kSpeedPresetValues[id];

    // 程序设置滑块（阻止信号递归）
    m_speedSlider->blockSignals(true);
    m_speedSlider->setValue(speedToSlider(speed));
    m_speedSlider->blockSignals(false);

    // 更新状态
    m_currentSpeed = speed;
    m_speedLabel->setText(kSpeedPresetLabels[id]);
    emit speedChanged(speed);
}

void SimControlPanel::onSliderMoved(int value)
{
    setSpeedBySlider(value);
}

void SimControlPanel::setSpeedBySlider(int sliderValue)
{
    int nearest = findNearestPreset(sliderValue);

    double speed;
    QString label;

    if (nearest >= 0) {
        // 接近预设档位 → 吸附到该档位
        speed = kSpeedPresetValues[nearest];
        label = kSpeedPresetLabels[nearest];
    } else {
        // 自定义位置 → 显示实际计算值
        speed = sliderToSpeed(sliderValue);
        label = formatSpeed(speed);
    }

    m_currentSpeed = speed;
    m_speedLabel->setText(label);
    emit speedChanged(speed);

    // 同步按钮状态（阻止信号递归）
    m_speedGroup->blockSignals(true);
    if (nearest >= 0) {
        m_btnPreset[nearest]->setChecked(true);
    } else {
        m_btnCustom->setChecked(true);
    }
    m_speedGroup->blockSignals(false);
}
