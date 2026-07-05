cmake_minimum_required(VERSION 3.19)
project(redstone-2d LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(redstone-2d
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h

    ui/SideBar.cpp
    ui/SideBar.h
    ui/MenuBar.cpp
    ui/MenuBar.h

    ui/pages/WelcomePage.cpp
    ui/pages/WelcomePage.h
    ui/pages/DesignPage.cpp
    ui/pages/DesignPage.h
    ui/pages/HelpPage.cpp
    ui/pages/HelpPage.h
    ui/pages/SettingsPage.cpp
    ui/pages/SettingsPage.h

    ui/pages/design_page/ComponentPanel.cpp
    ui/pages/design_page/ComponentPanel.h
    ui/pages/design_page/SimControlPanel.cpp
    ui/pages/design_page/SimControlPanel.h
    ui/pages/design_page/EditCanvas.cpp
    ui/pages/design_page/EditCanvas.h
)

target_link_libraries(redstone-2d
    PRIVATE
        Qt::Core
        Qt::Widgets
)

# 等价于 WIN32：不显示控制台窗口
target_link_options(redstone-2d PRIVATE "-Wl,--subsystem,windows")


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
