# ─── 自定义场景测试：特定布局/存档驱动的验证 ───
# 未来存档功能就绪后：读入布局文件 → 断言信号状态，可批量回归
# 命名：test_<主题>.cpp（Qt 向导默认 tst_，本项目统一 test_ 前缀）
# 新增：把相对路径加入下方列表（文件名即测试名），然后重新配置
set(CUSTOM_TEST_SOURCES
    # test_LevelX.cpp          # 从存档/布局文件加载的关卡场景
)

foreach(test_src IN LISTS CUSTOM_TEST_SOURCES)
    rsd_add_test(${test_src})
endforeach()
