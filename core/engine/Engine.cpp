#include "Engine.h"

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::processTick(World *world)
{
    // ─── 骨架阶段：空实现，后续实现 tick 三阶段 ───
    // 1. Phase 1 — 清空全部输出，信号源赋初值
    // 2. Phase 2 — BFS 传播
    // 3. Phase 3 — 消费者响应
    (void)world;
}
