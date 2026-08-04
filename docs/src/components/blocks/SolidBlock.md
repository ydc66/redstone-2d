# SolidBlock — 实心方块

> 对应源码：`components/blocks/SolidBlock.h/.cpp`
> 单元测试：`tests/unit/components/blocks/test_SolidBlock.cpp`（6 用例全绿）

## 1. 职责定位

实心方块是基础结构元件：`Category::Solid`，四方向均可被充能与传导信号，**不衰减**。同时是强弱充能规则的关键枢纽——弱充能不能激活红石粉，强充能可以。内部维护材质切换状态（交互循环 3 种材质），与信号行为无关。

## 2. 接口清单

| 接口 | 说明 |
|---|---|
| `Category category()` | `Solid` |
| `bool isTransceiver()` | true（Phase 2 BFS 计算输出） |
| `bool isStrongOutput()` | 动态：`m_strongPowered`——被任一强充能输入时整体输出强充能 |
| `void computeOutput(GridModel*)` | 四方向取输入 max 不衰减输出；聚合时同时记录是否含强充能 |
| `void onInteract()` | 切换材质索引（3 种循环） |

## 3. 设计契约

- **不衰减传导**：输出强度 = 输入最大值，无逐格衰减（与红石粉的 `max-1` 对比）
- **强弱充能聚合**：任一方向收到强充能 → `isStrongOutput()=true`；强度取四方向 max。混合输入（强 7 + 弱 15）→ 输出 15 且强充能
- **防自激过滤**：`computeOutput` 跳过火把（RedstoneTorchBase）与其他实心方块邻居——防止方块被自身传导的火把信号或方块链循环充能
- **弱充能语义**：未被强充能时输出为弱充能，因此不能激活相邻红石粉（粉侧规则）；被强充能后输出强充能，可激活粉
- **材质交互独立**：`onInteract()` 只改 `m_currentIdx`，不触碰信号状态

## 4. 测试验证

`test_SolidBlock.cpp` 覆盖：

| 用例 | 验证点 |
|---|---|
| solid_transmitsWithoutDecay | 强充能 15 不衰减传导，接收端收到 15 强信号 |
| solid_weakPower_cannotActivateDust | 弱充能注入方块 → 相邻粉输出 0（核心规则） |
| solid_strongPower_activatesDust | 强充能注入方块 → 相邻粉激活输出 14 |
| solid_ignoresTorchNeighbor | 火把邻居信号被过滤，方块输出 0（防自激） |
| solid_mixedPower_takesMax | 强 7 + 弱 15 → 输出 15 且强充能 |
| solid_interact_cyclesMaterial | 材质切换不破坏状态 |

## 5. 变更历史

| 日期 | 变更 |
|---|---|
| 2026-08-04 | 首版单元测试与文档（强弱充能规则 + 防自激过滤） |
