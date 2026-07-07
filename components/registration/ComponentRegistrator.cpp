#include "ComponentRegistrator.h"

#include "components/blocks/SolidBlock.h"

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
}

// ═══════════════════════════════════════════════════════════
//  程序启动时自动注册一次（静态初始化）
// ═══════════════════════════════════════════════════════════
static bool s_registered = (registerComponents(), true);
