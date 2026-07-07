#include "Engine.h"

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::processTick()
{
    // ─── 骨架阶段：空实现，后续实现 tick 三阶段 ───
    // 1. collectInputs — 收集输入信号
    // 2. onTick       — 计算输出
    // 3. commitState  — 提交状态
}
