#pragma once

/**
 * @brief 仿真引擎
 *
 * 管理红石电路的 tick 模拟。
 * 当前为骨架占位，后续实现 Phase 1~3 信号传播。
 */
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    /// 执行一个 tick
    void processTick(class World *world);

private:
    // 运行状态由 World 统一管理
};
