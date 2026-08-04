# Lever — 拉杆

> 对应源码：`components/sources/Lever.h/.cpp`
> 单元测试：`tests/unit/components/sources/test_Lever.cpp`（3 用例全绿）

## 1. 职责定位

交互式信号源：点击切换 ON/OFF。ON 时向四方向输出**强充能 15**，OFF 时无输出。`Category::NonSolid`，无输入端口，不接收外界信号。

## 2. 接口清单

| 接口 | 说明 |
|---|---|
| `Category category()` | `NonSolid` |
| `bool isSignalSource()` | true（Phase 1 按 basePowerLevel 设输出） |
| `bool isStrongOutput()` | true，拉杆 = 强充能源 |
| `int basePowerLevel()` | `m_powered ? 15 : 0` |
| `void onInteract()` | 翻转 `m_powered` |
| `bool isPowered()` | 当前开关状态查询 |

## 3. 设计契约

- **状态即输出**：引擎 Phase 1 每 tick 重读 `basePowerLevel()`，交互后无需额外 tick 即生效（无延迟）
- **强充能语义**：输出 `isStrong=true`，可激活红石粉、充能实心方块（强充能路径，见 [RedstoneSignal.md](../../core/meta_component/RedstoneSignal.md)）
- **四向输出**：默认四方向输出端口，端口随 facing 声明（本类固定 North facing + 四向 RelDir）

## 4. 测试验证

`test_Lever.cpp` 覆盖：

| 用例 | 验证点 |
|---|---|
| lever_initialOff | 初始 OFF，输出 0 |
| lever_toggleOn_outputsStrong15 | 交互打开 → 输出 15 强充能，接收端收到 `{15, strong}` |
| lever_toggleOff_backToZero | 再交互关闭 → 输出回 0 |

## 5. 变更历史

| 日期 | 变更 |
|---|---|
| 2026-08-04 | 首版单元测试与文档 |
