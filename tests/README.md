# 测试目录（tests/）

> 总体导航：测试按**测试范围**分类组织，而不是把文件平铺在一个目录里。

## 一、目录结构

| 子目录 | 定位 | 未来示例 |
|--------|------|----------|
| `unit/` | **模块单元测试**：单个类/模块的行为验证；**内部镜像源码目录结构**（见下） | core/meta_component/ 的 Direction、core/model/ 的 GridModel、components/ 各元件 |
| `scenario/` | **多元件场景测试**：真实电路集成行为 | 反相器、RS 锁存器、脉冲链、长链信号传播、引擎三阶段全链路 |
| `custom/` | **自定义场景测试**：特定布局/存档驱动的验证 | 读入存档布局 → 断言信号状态（未来存档功能就绪后更便捷） |

### unit/ 镜像源码目录结构

`tests/unit/` 内部与项目根源码目录**一一对应**，被测文件在哪，测试文件就在哪：

```
tests/unit/
├── core/
│   ├── meta_component/    test_Direction.cpp、test_Component.cpp
│   ├── model/             test_GridModel.cpp
│   └── engine/            test_Engine.cpp
└── components/
    ├── blocks/            test_SolidBlock.cpp
    ├── transmission/      test_RedstoneDust.cpp、test_RedstoneTorch.cpp
    ├── sources/           test_Lever.cpp
    └── ...
```

> 物理目录同步建立（新建文件时创建镜像子目录）；`unit/CMakeLists.txt` 的源列表用**相对路径**（`core/meta_component/test_Direction.cpp`）显式声明。

## 二、命名与注册约定

- 测试文件：`test_<主题>.cpp`，一个文件一个 QtTest 类，使用 `QTEST_GUILESS_MAIN`（纯逻辑，无需 QApplication）
- **测试名 = 文件名**：`test_Direction.cpp` → 测试名 `test_Direction`，由 `rsd_add_test()` 统一注册
- 前缀说明：Qt 向导默认 `tst_`，本项目统一用 `test_`（纯命名偏好，无技术约束）
- 每个子目录的 `CMakeLists.txt` 显式列出本类测试文件（**不用 GLOB**）

### 最小模板

```cpp
#include <QtTest>
#include "core/model/GridModel.h"

class test_Example : public QObject {
    Q_OBJECT
private slots:
    void init() { /* 每个用例执行前调用，可放 World 重建 */ }
    void exampleCase() { QVERIFY(true); }
};

QTEST_GUILESS_MAIN(test_Example)
#include "test_Example.moc"
```

## 三、如何新增一个测试

1. 新建 `test_xxx.cpp`（模板见上），**存放于镜像源码路径的子目录**（如 `unit/core/meta_component/test_Direction.cpp`）
2. 把相对路径（`core/meta_component/test_Direction.cpp`）加入 `unit/CMakeLists.txt` 的源列表
3. 重新配置 + 构建 + 运行：

```powershell
cmake -S <ROOT> -B <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug
cmake --build <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug
ctest --test-dir <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug --output-on-failure
```

4. 单个测试调试（QtTest 输出在 PowerShell 下会被吞，重定向到文件）：

```powershell
cd <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug
.\test_xxx.exe -o result.log
Get-Content result.log
```

## 四、运行与报告

### 方式 1：按目录运行

每个分类目录在构建树里都有独立的 `CTestTestfile.cmake`，`--test-dir` 指到哪层就只跑哪层：

```powershell
ctest --test-dir <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug\tests\unit        # 只跑单元测试
ctest --test-dir <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug\tests\scenario   # 只跑场景测试
ctest --test-dir <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug\tests\custom     # 只跑自定义场景
```

### 方式 3：按标签运行

每个测试**自动带"所在子目录名"标签**（unit / scenario / custom）；可附加标签（如性能）：

```cmake
# scenario/CMakeLists.txt：给性能用例打 perf 标签
rsd_add_test(test_Performance.cpp "perf")   # 实际标签：scenario;perf
```

```powershell
ctest --test-dir <构建目录> -L perf          # 只跑性能测试
ctest --test-dir <构建目录> -LE perf         # 日常跑，排除性能（报告仍可用 -L 单独出）
ctest --test-dir <构建目录> -L unit          # 等价于方式 1 的 unit 目录
```

两种方式可叠加：`ctest --test-dir <构建目录>\tests\scenario -L perf`（场景目录里再筛性能）。

### 全量运行与报告

```powershell
# 全部测试（当前框架阶段：0 个用例，后续逐步补充）
ctest --test-dir <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug

# 失败详情
ctest --test-dir <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug --output-on-failure

# 生成 JUnit 格式报告（可上传 CI / 存档）
ctest --test-dir <ROOT>\build\Desktop_Qt_6_11_1_MinGW_64_bit-Debug --output-junit report.xml
```

报告覆盖范围（全部注册在 ctest 下，一条命令出全量报告）：
- **基本功能**：unit/ + scenario/ 的断言类测试
- **整体性能**：性能回归用例（大网格 tick 耗时上限断言，如 32×32 满布元件）
- **自定义场景**：custom/ 的存档布局测试

## 五、约定

- 新增测试文件必须同时更新对应子目录的 `CMakeLists.txt`（显式清单，不自动收集）
- 测试只链接 `redstone-2d-components`（PUBLIC 传播 core），**不依赖 UI 层**
