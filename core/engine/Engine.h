#pragma once

/**
 * @brief 仿真引擎骨架
 *
 * 管理红石电路的 tick 模拟。
 * 当前为骨架占位，后续实现 tick 三阶段（collectInputs → onTick → commitState）。
 */
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    /// 执行一个 tick
    void processTick();

    /// 引擎是否正在运行
    bool isRunning() const { return m_running; }

private:
    bool m_running = false;
};
