# RedstoneLamp — 红石灯

> 对应源码：`components/mechanical/RedstoneLamp.h/.cpp`
> 单元测试：`tests/unit/components/mechanical/test_RedstoneLamp.cpp`（3 用例全绿）

## 1. 职责定位

典型消费者元件：四方向接收信号，任一方向强度 > 0 即点亮。`Category::NonSolid`，无输出端口，不传播信号。

## 2. 接口清单

| 接口 | 说明 |
|---|---|
| `Category category()` | `NonSolid` |
| `bool isConsumer()` | true（Phase 3 读取输入信号） |
| `void onTick(const std::array<RedstoneSignal,4>&)` | 遍历输入端口，任一强度 > 0 → `m_lit = true` |
| `bool isLit()` | 当前亮灭状态查询 |

## 3. 设计契约

- **即时响应**：`onTick` 直接以当前 tick 的信号缓存决定亮灭，无延迟（与火把的延迟状态机对比）
- **端口过滤**：仅检查 `canInputFrom(dir)` 的端口方向，非端口方向的信号被忽略
- **强弱充能一视同仁**：只要强度 > 0 即点亮，不区分强弱（消费者语义，见 [Component.md](../../core/meta_component/Component.md)）

## 4. 测试验证

`test_RedstoneLamp.cpp` 覆盖：

| 用例 | 验证点 |
|---|---|
| lamp_unpowered_off | 无信号 → 灭 |
| lamp_powered_lit | 相邻源 15 → 亮 |
| lamp_powerLost_unlit | 源断电 → 立即熄灭 |

## 5. 变更历史

| 日期 | 变更 |
|---|---|
| 2026-08-04 | 首版单元测试与文档 |
