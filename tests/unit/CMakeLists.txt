# ─── 单元测试：单个类/模块的行为验证 ───
# 目录结构镜像源码（tests/unit/ 对应项目根，文件存放于镜像子目录）：
#   unit/core/meta_component/   方向、组件基类
#   unit/core/model/            网格模型
#   unit/core/engine/           引擎
#   unit/components/...         各元件（按 components/ 分类子目录）
# 命名：test_<主题>.cpp（Qt 向导默认 tst_，本项目统一 test_ 前缀）
# 新增：把相对路径加入下方列表（文件名即测试名），然后重新配置
set(UNIT_TEST_SOURCES
    core/meta_component/test_Direction.cpp   # 方向系统：四方向取余、toRelativeDir/toAbsoluteDir 互逆
    core/meta_component/test_WorldObject.cpp # 网格对象基类：占位、朝向旋转、渲染模板方法
    components/test/test_UniversalProbe.cpp  # 万能探针：准确信号源、按方向取信号
    components/transmission/test_RedstoneDust.cpp   # 红石粉：衰减、端口方向性、弱充能过滤
    components/transmission/test_RedstoneTorch.cpp  # 火把：延迟状态机、永亮/附着销毁
    components/blocks/test_SolidBlock.cpp    # 实心方块：不衰减传导、强弱充能规则、火把邻居过滤
    components/blocks/test_DirectionalBlock.cpp  # 方向箭头方块：朝向旋转交互
    components/sources/test_Lever.cpp        # 拉杆：交互切换、强充能 15 输出
    components/sources/test_RedstoneBlock.cpp # 红石块：恒定强充能 15
    components/mechanical/test_RedstoneLamp.cpp # 红石灯：消费者响应（亮/灭）
    # core/meta_component/test_Component.cpp   # 组件基类：facing、canInputFrom/canOutputTo 端口映射
    # core/model/test_GridModel.cpp            # 网格模型：放置/移除/越界/resize
)

foreach(test_src IN LISTS UNIT_TEST_SOURCES)
    rsd_add_test(${test_src})
endforeach()
