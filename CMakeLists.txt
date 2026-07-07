cmake_minimum_required(VERSION 3.19)
project(redstone-2d LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# ─── 自动扫描所有源文件 ───
# 递归查找所有 .cpp 和 .h，新增文件后重新运行 cmake 即可
file(GLOB_RECURSE SOURCES *.cpp *.h)
list(FILTER SOURCES EXCLUDE REGEX "/build/")
list(FILTER SOURCES EXCLUDE REGEX "/\\.")

qt_add_executable(redstone-2d
    MACOSX_BUNDLE
    ${SOURCES}
)

target_link_libraries(redstone-2d
    PRIVATE
        Qt::Core
        Qt::Widgets
)

# 添加项目根目录到包含路径，保证 #include "core/..." 等写法可用
target_include_directories(redstone-2d PRIVATE ${CMAKE_SOURCE_DIR})

install(TARGETS redstone-2d
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET redstone-2d
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
