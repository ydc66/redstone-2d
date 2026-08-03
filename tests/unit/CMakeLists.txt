# ─── 单元测试：单个类/模块的行为验证 ───
# 新增：把 tst_xxx.cpp 加入下方列表（文件名即测试名），然后重新配置
set(UNIT_TEST_SOURCES
    # tst_Direction.cpp        # 方向系统：四方向取余、toRelativeDir/resolveDir 互逆
    # tst_GridModel.cpp        # 网格模型：放置/移除/越界/resize
    # tst_RedstoneDust.cpp     # 红石粉：衰减链、十字/一字导通
    # tst_RedstoneTorch.cpp    # 火把：延迟状态机、Back 触发防自激
    # tst_SolidBlock.cpp       # 实心方块：混合充能、强充能直连
)

foreach(test_src IN LISTS UNIT_TEST_SOURCES)
    rsd_add_test(${test_src})
endforeach()
