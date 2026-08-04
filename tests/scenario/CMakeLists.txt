# ─── 场景测试：多元件集成，验证真实电路行为 ───
# 新增：把 tst_xxx.cpp 加入下方列表（文件名即测试名），然后重新配置
# gates/ 子目录：组合逻辑门（拉杆/实心方块/附着火把/红石粉/红石灯），
# 共享辅助 TestCircuit.h（放置/查询/运行），只依赖 GridModel + Engine
set(SCENARIO_TEST_SOURCES
    gates/test_NotGate.cpp        # 非门：拉杆 → 方块 → 火把 → 粉 → 灯
    gates/test_OrGate.cpp         # 或门：两拉杆 → 两粉 → 十字粉 → 灯
    gates/test_NandGate.cpp       # 与非门：两路“拉杆→方块→火把→粉”汇聚十字粉 → 灯
    gates/test_AndGate.cpp        # 与门：NAND + 非门（粉 → 方块 → 火把 → 灯）
    # test_Latch.cpp             # RS 锁存器 / 脉冲电路（需火把间接反馈，待元件支持后补）
    # test_Engine.cpp            # 引擎三阶段全链路、惰性传播、信号快照
)

# 性能回归单独打 perf 标签（日常可用 ctest -LE perf 排除，报告仍包含）：
# rsd_add_test(test_Performance.cpp "perf")

foreach(test_src IN LISTS SCENARIO_TEST_SOURCES)
    rsd_add_test(${test_src})
endforeach()
