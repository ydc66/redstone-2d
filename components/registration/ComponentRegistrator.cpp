#include "ComponentRegistrator.h"

#include "components/blocks/SolidBlock.h"
#include "components/blocks/DirectionalBlock.h"
#include "components/sources/RedstoneBlock.h"
#include "components/transmission/RedstoneDust.h"
#include "components/mechanical/RedstoneLamp.h"

#include "ComponentRegistry.h"

void registerComponents()
{
    static bool once = false;
    if (once) return;
    once = true;

    // ─── 纯方块：实心方块 ───
    ComponentRegistry::instance().registerType<SolidBlock>(
        QStringLiteral("solid_block"),       // id
        QStringLiteral("实心方块"),           // name
        QStringLiteral("纯方块")              // group
    );

    // ─── 定向方块：方向箭头 ───
    ComponentRegistry::instance().registerType<DirectionalBlock>(
        QStringLiteral("directional_block"), // id
        QStringLiteral("方向箭头"),           // name
        QStringLiteral("纯方块")              // group
    );

    // ─── 信号源：红石块 ───
    ComponentRegistry::instance().registerType<RedstoneBlock>(
        QStringLiteral("redstone_block"),     // id
        QStringLiteral("红石块"),             // name
        QStringLiteral("信号源")              // group
    );

    // ─── 传输线：红石粉 ───
    ComponentRegistry::instance().registerType<RedstoneDust>(
        QStringLiteral("redstone_dust"),      // id
        QStringLiteral("红石粉"),             // name
        QStringLiteral("传输元件")            // group
    );

    // ─── 机械元件：红石灯 ───
    ComponentRegistry::instance().registerType<RedstoneLamp>(
        QStringLiteral("redstone_lamp"),      // id
        QStringLiteral("红石灯"),             // name
        QStringLiteral("机械元件")            // group
    );
}

// ═══════════════════════════════════════════════════════════
//  程序启动时自动注册一次（静态初始化）
// ═══════════════════════════════════════════════════════════
static bool s_registered = (registerComponents(), true);
