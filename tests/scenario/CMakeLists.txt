# ─── 场景测试：多元件集成，验证真实电路行为 ───
# 新增：把 tst_xxx.cpp 加入下方列表（文件名即测试名），然后重新配置
set(SCENARIO_TEST_SOURCES
    # tst_Inverter.cpp         # 反相器：拉杆 → 方块 → 火把 → 粉 → 灯 延迟链
    # tst_Latch.cpp            # RS 锁存器 / 脉冲电路
    # tst_Engine.cpp           # 引擎三阶段全链路、惰性传播、信号快照
)

# 性能回归单独打 perf 标签（日常可用 ctest -LE perf 排除，报告仍包含）：
# rsd_add_test(tst_Performance.cpp "perf")

foreach(test_src IN LISTS SCENARIO_TEST_SOURCES)
    rsd_add_test(${test_src})
endforeach()
