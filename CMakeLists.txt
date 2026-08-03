cmake_minimum_required(VERSION 3.19)
project(redstone-2d LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Test)

qt_standard_project_setup()

# ─── 分层库：core → components → ui（依赖单向；测试只依赖前两层）───
add_subdirectory(core)
add_subdirectory(components)
add_subdirectory(ui)

# ─── 主程序（应用壳：入口 + 主窗口装配）───
qt_add_executable(redstone-2d
    MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

# redstone-2d-ui 的 PUBLIC 依赖会连带传播 components / core 的 include 与链接
target_link_libraries(redstone-2d
    PRIVATE
        redstone-2d-ui
        Qt::Widgets
)

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

# ─── 单元测试（ctest 一键运行；enable_testing 需在 add_subdirectory(tests) 之前）───
enable_testing()
add_subdirectory(tests)
