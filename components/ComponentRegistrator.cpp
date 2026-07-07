#include "ComponentRegistrator.h"

#include "blocks/SolidBlock.h"

#include "core/ComponentRegistry.h"
#include "core/meta_component/ComponentSpec.h"

void registerComponents()
{
    static bool once = false;
    if (once) return;
    once = true;

    // ─── 纯方块：实心方块 ───
    ComponentSpec spec;
    spec.category       = Category::Solid;
    spec.isPushable     = false;
    spec.basePowerLevel = 0;

    ComponentRegistry::instance().registerType(
        QStringLiteral("solid_block"),       // id
        QStringLiteral("实心方块"),           // name
        QStringLiteral("纯方块"),             // group
        spec,
        createSolidBlock);                   // 工厂函数
}

// ═══════════════════════════════════════════════════════════
//  程序启动时自动注册一次（静态初始化）
// ═══════════════════════════════════════════════════════════
static bool s_registered = (registerComponents(), true);
