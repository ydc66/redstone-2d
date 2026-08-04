# RedstoneTorch — 红石火把家族

> 对应源码：`components/transmission/RedstoneTorchBase.h/.cpp`、`RedstoneTorchGround.h/.cpp`、`RedstoneTorchWall.h/.cpp`
> 单元测试：`tests/unit/components/transmission/test_RedstoneTorch.cpp`（6 用例全绿）

## 1. 职责定位

红石火把是延迟信号源：点亮时向四方向输出弱充能 15。家族由抽象基类 `RedstoneTorchBase` 与两个具体子类组成：

- **RedstoneTorchBase（抽象基类）**：内置延迟状态机，输出统一弱充能 15；子类只需声明端口 + 实现 `paintContent()`
- **RedstoneTorchGround（地基火把）**：无输入端口，永远点亮，纯粹的信号源角色
- **RedstoneTorchWall（附着火把）**：输入端口位于 Back（附着方向），被背面充能时熄灭；每 tick 校验附着，悬空自毁

## 2. 接口清单

| 接口 | 说明 |
|---|---|
| `Category category()` | `NonSolid` |
| `bool isTransceiver()` / `isConsumer()` | 均 true（参与 BFS 输出计算 + tick 输入读取） |
| `bool isStrongOutput()` | false，输出弱充能 |
| `bool isLit()` | 当前点亮状态查询 |
| `int tickDelay()` / `void setTickDelay(int)` | 状态切换延迟 tick 数（0~100，默认 1，可动态调整用于测试调试） |
| `bool isMarkedForRemoval()`（仅 Wall） | 附着校验失败后置 true |

## 3. 设计契约

- **延迟状态机时序**：`onTick()`（Phase 3）读取输入信号 → 写入待决状态 `m_pendingLit`；下一次 tick 的 `computeOutput()`（Phase 2）延迟切换 `m_lit`。即：信号到达后需再经过 `tickDelay` 个 tick 才实际切换输出
- **熄灭条件**：有且只有输入端口中任一方向收到强度 > 0 信号时待熄灭（`m_pendingLit = !hasSignal`）
- **Ground 永不熄灭**：无输入端口 → 每 tick 读不到信号 → 待决状态恒为点亮，旁边放强源也不受影响
- **Wall 附着契约**：Back 绝对方向（默认 facing=South 时 Back=North）邻居必须是实心方块，否则 `computeOutput` 标记 `isMarkedForRemoval` 并输出 0；附着校验先于状态机执行
- **防自激**：输出恒为弱充能，无法反向激活自身

## 4. 测试验证

`test_RedstoneTorch.cpp` 覆盖（Wall 测试布局：SolidBlock(1,0) + Wall(1,1) + Lever(0,0) 控制背面充能）：

| 用例 | 验证点 |
|---|---|
| ground_alwaysLit_evenWithNeighborSignal | 旁边强源下仍点亮、输出 15 |
| torch_lightsOff_afterDelay | 背面充能后延迟 1 tick 熄灭 |
| torch_lightsOn_afterDelay | 失能后延迟 1 tick 恢复点亮 |
| torch_customDelay | `setTickDelay(2)` 时需 2 tick 过渡期 |
| wall_attachedToSolid_works | 附着实心方块时正常点亮、不销毁 |
| wall_missingAttachment_markedRemoval | 悬空或附着非实心元件 → 标记销毁、输出 0 |

## 5. 变更历史

| 日期 | 变更 |
|---|---|
| 2026-08-04 | 首版单元测试与文档（延迟时序按 tick 逐拍断言） |
