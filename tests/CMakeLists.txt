# ─── 测试总控：三类子目录 + 统一注册函数 ───
# 分类（详见 tests/README.md）：
#   unit/     模块单元测试（单个类/模块）
#   scenario/ 多元件集成场景测试（真实电路行为）
#   custom/   自定义场景测试（未来存档/导入驱动）

# 测试运行时需要 Qt 与 MinGW 运行库在 PATH 中（否则 0xc0000135 DLL not found）
get_target_property(QT6_CORE_LOCATION Qt6::Core LOCATION)
get_filename_component(QT6_BIN_DIR "${QT6_CORE_LOCATION}" DIRECTORY)
get_filename_component(TOOLCHAIN_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
# ENVIRONMENT_MODIFICATION 是列表属性：路径间分隔符需转义为字面分号
set(TEST_PATH_PREFIX "${QT6_BIN_DIR};${TOOLCHAIN_BIN_DIR}")
string(REPLACE ";" "\\;" TEST_PATH_PREFIX "${TEST_PATH_PREFIX}")

# 统一注册函数：文件名 tst_xxx.cpp → 测试名 tst_xxx
# 供各子目录 CMakeLists 调用；测试只链 components（PUBLIC 传播 core），不依赖 UI
# 标签：自动打上"所在子目录名"（unit/scenario/custom），可附加标签，例如：
#   rsd_add_test(tst_Performance.cpp "perf")  → 标签 scenario;perf
#   ctest -L perf 只跑性能；ctest -LE perf 日常排除性能
function(rsd_add_test test_src)
    get_filename_component(auto_label ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    set(test_labels ${auto_label} ${ARGN})
    get_filename_component(test_name ${test_src} NAME_WE)
    qt_add_executable(${test_name} ${test_src})
    # 测试用控制台子系统：stdout 可见，ctest 可捕获失败详情
    set_target_properties(${test_name} PROPERTIES WIN32_EXECUTABLE OFF)
    target_link_libraries(${test_name} PRIVATE redstone-2d-components Qt::Test)
    add_test(NAME ${test_name} COMMAND ${test_name})
    # ctest 运行环境自动带上 Qt bin + 工具链 bin；LABELS 支持 -L / -LE 筛选
    set_tests_properties(${test_name} PROPERTIES
        ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${TEST_PATH_PREFIX}"
        LABELS "${test_labels}")
endfunction()

add_subdirectory(unit)
add_subdirectory(scenario)
add_subdirectory(custom)
