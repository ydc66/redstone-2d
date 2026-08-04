# RedstoneBlock — 红石块

> 对应源码：`components/sources/RedstoneBlock.h/.cpp`
> 单元测试：`tests/unit/components/sources/test_RedstoneBlock.cpp`（1 用例全绿）

## 1. 职责定位

恒定信号源：始终向四方向输出**强充能 15**。`Category::NonSolid`，无输入端口，不可交互，永不变化。

## 2. 接口清单

| 接口 | 说明 |
|---|---|
| `Category category()` | `NonSolid` |
| `bool isSignalSource()` | true（Phase 1 设输出） |
| `bool isStrongOutput()` | true |
| `int basePowerLevel()` | 恒 15 |

## 3. 设计契约

- **恒定输出**：`basePowerLevel()` 恒为 15，Phase 1 每 tick 重设，Phase 2 无需计算（不重写 computeOutput）
- **强充能语义**：输出 `isStrong=true`，可激活红石粉与充能实心方块
- **无状态**：不可交互、无输入端口，是测试与搭建中最可靠的基准源（与探针 Source 的"可配置"相比是固定值）

## 4. 测试验证

`test_RedstoneBlock.cpp` 覆盖：

| 用例 | 验证点 |
|---|---|
| redstoneBlock_constantStrong15 | 输出恒定 15 强充能，接收端收到 `{15, strong}` |

## 5. 变更历史

| 日期 | 变更 |
|---|---|
| 2026-08-04 | 首版单元测试与文档 |
