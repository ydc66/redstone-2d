# RedstoneDust — 红石粉家族

> 对应源码：`components/transmission/RedstoneDust.h/.cpp`、`RedstoneDustCross.h/.cpp`、`RedstoneDustLine.h/.cpp`
> 单元测试：`tests/unit/components/transmission/test_RedstoneDust.cpp`（7 用例全绿）

## 1. 职责定位

红石粉是基础传输元件，信号沿粉线每格衰减 1 强度。家族由抽象基类 `RedstoneDust` 与两个具体子类组成：

- **RedstoneDust（抽象基类）**：共享衰减传播逻辑。`computeOutput()` 遍历构造时声明的输入端口，取邻居信号最大值输出 `max-1`
- **RedstoneDustCross（十字粉）**：四向输入/输出，facing 不影响功能
- **RedstoneDustLine（一字粉）**：仅两端（Front/Back）输入/输出，`onInteract()` 顺时针旋转 90° 切换横/竖导线

## 2. 接口清单

| 接口 | 说明 |
|---|---|
| `Category category()` | `NonSolid`（信号可穿过） |
| `bool isTransceiver()` | true，中继元件（Phase 2 BFS 计算输出） |
| `bool isStrongOutput()` | false，输出为弱充能 |
| `void computeOutput(GridModel*)` | 输入端口取 max 后 `setOutputStrength(max(0, maxInput-1))` |
| `void onInteract()`（仅 Line） | 旋转 90°（facing 循环） |

## 3. 设计契约

- **每格衰减 1**：输出 = 输入最大值 − 1，且不低于 0（强度 1 的源经粉输出为 0，不出负数）
- **弱充能过滤规则**：实心方块的弱充能不能激活红石粉（`RedstoneDust.cpp` 中显式检查邻居 `isSolid()` 并跳过）；其他元件（含同级红石粉、火把）的弱充能可以激活。强充能无论来源均可激活
- **端口方向性**：`computeOutput` 用 `canInputFrom(dir)` 过滤——非端口方向的邻居信号被忽略。Line 只有两端是端口，侧面信号无效；旋转后端口随 facing 旋转（相对方向声明，见 [Component.md](../../core/meta_component/Component.md) 端口契约）
- **子类职责最小化**：子类只需构造时传入端口列表 + 实现 `paintContent()`

## 4. 测试验证

`test_RedstoneDust.cpp` 覆盖：

| 用例 | 验证点 |
|---|---|
| cross_decayByOne | 15 → 14 单格衰减，接收端收到 14 |
| cross_chainDecay | 双段链 15 → 14 → 13 逐格衰减 |
| cross_strengthOne_notNegative | 强度 1 输出 0，不为负 |
| cross_weakNonSolid_activates | 弱充能 7（非实心源）→ 输出 6，弱充能有效 |
| line_onlyEndsReceive | 侧面信号被端口过滤（0），端部信号正常（14） |
| line_rotate_swapsPorts | 旋转后侧面变端口、原端口失效 |
| line_interact_rotatesClockwise | 交互四步旋转循环 North→East→South→West |

## 5. 变更历史

| 日期 | 变更 |
|---|---|
| 2026-08-04 | 首版单元测试与文档（基于 UniversalProbe 三件套 + GridModel/Engine 直驱） |
