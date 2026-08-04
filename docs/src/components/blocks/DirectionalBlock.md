# DirectionalBlock — 方向箭头方块

> 对应源码：`components/blocks/DirectionalBlock.h/.cpp`
> 单元测试：`tests/unit/components/blocks/test_DirectionalBlock.cpp`（2 用例全绿）

## 1. 职责定位

纯装饰元件：显示箭头指向当前朝向，右键顺时针旋转 90°。`Category::NonSolid`，无任何输入/输出端口，不参与红石信号传播。

## 2. 接口清单

| 接口 | 说明 |
|---|---|
| `Category category()` | `NonSolid` |
| `void onInteract()` | `setFacing(rotateRight(facing()))` 顺时针旋转 90° |

## 3. 设计契约

- **朝向语义**：箭头朝北绘制，基类 `paint()` 根据 facing 旋转画布，使箭头指向实际朝向
- **旋转闭环**：`onInteract()` 四步循环 North → East → South → West → North
- **不参与信号**：非 transceiver/consumer/source，BFS 与 tick 均不处理

## 4. 测试验证

`test_DirectionalBlock.cpp` 覆盖：

| 用例 | 验证点 |
|---|---|
| directional_initialFacingNorth | 初始朝向 North、NonSolid |
| directional_interact_rotatesClockwise | 交互旋转四步循环回绕 |

## 5. 变更历史

| 日期 | 变更 |
|---|---|
| 2026-08-04 | 首版单元测试与文档 |
