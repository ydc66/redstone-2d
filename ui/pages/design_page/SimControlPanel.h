#ifndef SIMCONTROLPANEL_H
#define SIMCONTROLPANEL_H

#include <QWidget>

class QPushButton;
class QSlider;
class QButtonGroup;
class QLabel;

/**
 * @class   SimControlPanel
 * @brief   仿真控制面板
 *
 * 左侧面板的仿真控制区域，包含运行/步进按钮和速度调节。
 * 速度预设按钮与滑块双向联动：单击预设按钮 → 滑块移动，拖动滑块 → 自动选中自定义按钮。
 */
class SimControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SimControlPanel(QWidget *parent = nullptr);

    /// 当前速度倍率
    double speedValue() const { return m_currentSpeed; }

signals:
    /// 运行/暂停切换信号
    void runToggled(bool running);
    /// 步进信号
    void stepRequested();
    /// 速度倍率变化信号
    void speedChanged(double speed);

private slots:
    /// 速度按钮点击槽函数
    void onSpeedButtonClicked(int id);
    /// 滑块移动槽函数
    void onSliderMoved(int value);

private:
    void setupUI();
    /// 连接信号与槽
    void setupConnections();
    /// 根据滑块值设置速度
    void setSpeedBySlider(int sliderValue);

    /// 速度预设按钮数量
    static constexpr int kPresetCount   = 3;
    /// 自定义速度按钮的 ID
    static constexpr int kCustomId      = 3;
    /// 滑块最小值
    static constexpr int kSliderMin     = 0;
    /// 滑块最大值
    static constexpr int kSliderMax     = 100;

    QPushButton  *m_btnRun;
    QPushButton  *m_btnStep;
    QPushButton  *m_btnPreset[kPresetCount];
    QPushButton  *m_btnCustom;
    QButtonGroup *m_speedGroup;
    QSlider      *m_speedSlider;
    QLabel       *m_speedLabel;
    double        m_currentSpeed = 1.0;
};

#endif // SIMCONTROLPANEL_H
