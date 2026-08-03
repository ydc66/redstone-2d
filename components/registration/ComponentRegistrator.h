#pragma once

/// 程序启动时自动注册所有元件到 ComponentRegistry，无需手动调用
void registerComponents();

/// 链接标志：静态库拆库后，main 引用该符号以保留本文件的静态初始化注册
extern const bool g_componentRegistrationFlag;
