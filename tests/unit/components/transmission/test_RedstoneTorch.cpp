#include <QtTest>

#include "components/blocks/SolidBlock.h"
#include "components/sources/Lever.h"
#include "components/test/UniversalProbe.h"
#include "components/transmission/RedstoneTorchGround.h"
#include "components/transmission/RedstoneTorchWall.h"
#include "core/engine/Engine.h"
#include "core/model/GridModel.h"

/// 红石火把家族单元测试：延迟状态机、Back 触发熄灭、附着校验（Ground 永亮 / Wall 附着）
class test_RedstoneTorch : public QObject
{
    Q_OBJECT

private slots:
    // ─── Ground：永不熄灭 ───
    void ground_alwaysLit_evenWithNeighborSignal();

    // ─── 延迟状态机（Wall，Back 被充能熄灭） ───
    void torch_lightsOff_afterDelay();
    void torch_lightsOn_afterDelay();
    void torch_customDelay();

    // ─── Wall 附着校验 ───
    void wall_attachedToSolid_works();
    void wall_missingAttachment_markedRemoval();
};

/// 布局：Wall(1,1) 背面朝上附着 SolidBlock(1,0)，Lever(0,0) 给方块充能
static void setupWallCircuit(GridModel *grid, RedstoneTorchWall **wallOut, Lever **leverOut)
{
    grid->resize(4, 3);

    auto block = std::make_unique<SolidBlock>(1, 0);
    grid->placeComponent(1, 0, std::move(block));

    auto wall = std::make_unique<RedstoneTorchWall>(1, 1);
    *wallOut = wall.get();
    grid->placeComponent(1, 1, std::move(wall));

    auto lever = std::make_unique<Lever>(0, 0);
    *leverOut = lever.get();
    grid->placeComponent(0, 0, std::move(lever));
}

void test_RedstoneTorch::ground_alwaysLit_evenWithNeighborSignal()
{
    GridModel grid;
    grid.resize(4, 3);

    auto torch = std::make_unique<RedstoneTorchGround>(1, 1);
    auto *torchPtr = torch.get();
    grid.placeComponent(1, 1, std::move(torch));

    // 旁边放强源：Ground 无输入端口，不受任何影响
    auto source = std::make_unique<UniversalProbe>(1, 0, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    grid.placeComponent(1, 0, std::move(source));

    Engine engine;
    engine.processTick(&grid);

    QVERIFY(torchPtr->isLit());               // 永远点亮
    QCOMPARE(torchPtr->outputStrength(), 15); // 弱充能 15
}

void test_RedstoneTorch::torch_lightsOff_afterDelay()
{
    GridModel grid;
    RedstoneTorchWall *wall = nullptr;
    Lever *lever = nullptr;
    setupWallCircuit(&grid, &wall, &lever);

    Engine engine;
    engine.processTick(&grid);   // tick 1：初始点亮，无输入
    QVERIFY(wall->isLit());
    QCOMPARE(wall->outputStrength(), 15);

    lever->onInteract();         // 打开拉杆 → 背面方块被充能
    engine.processTick(&grid);   // tick 2：onTick 读到信号 → 待熄灭
    QVERIFY(wall->isLit());      // 延迟 1 tick，仍亮
    QCOMPARE(wall->outputStrength(), 15);

    engine.processTick(&grid);   // tick 3：延迟结束，熄灭
    QVERIFY(!wall->isLit());
    QCOMPARE(wall->outputStrength(), 0);
}

void test_RedstoneTorch::torch_lightsOn_afterDelay()
{
    GridModel grid;
    RedstoneTorchWall *wall = nullptr;
    Lever *lever = nullptr;
    setupWallCircuit(&grid, &wall, &lever);

    Engine engine;
    lever->onInteract();
    engine.processTick(&grid);   // tick 1
    engine.processTick(&grid);   // tick 2：已熄灭
    QVERIFY(!wall->isLit());

    lever->onInteract();         // 关闭拉杆 → 背面失去信号
    engine.processTick(&grid);   // tick 3：onTick 读到无信号 → 待点亮
    QVERIFY(!wall->isLit());     // 延迟 1 tick，仍灭

    engine.processTick(&grid);   // tick 4：延迟结束，恢复点亮
    QVERIFY(wall->isLit());
    QCOMPARE(wall->outputStrength(), 15);
}

void test_RedstoneTorch::torch_customDelay()
{
    GridModel grid;
    RedstoneTorchWall *wall = nullptr;
    Lever *lever = nullptr;
    setupWallCircuit(&grid, &wall, &lever);
    wall->setTickDelay(2);       // 自定义 2 tick 延迟

    Engine engine;
    lever->onInteract();
    engine.processTick(&grid);   // tick 1：读到信号
    QVERIFY(wall->isLit());      // 过渡第 1 tick
    engine.processTick(&grid);   // tick 2
    QVERIFY(wall->isLit());      // 过渡第 2 tick，仍未切换
    engine.processTick(&grid);   // tick 3：延迟满足，熄灭
    QVERIFY(!wall->isLit());
}

void test_RedstoneTorch::wall_attachedToSolid_works()
{
    GridModel grid;
    RedstoneTorchWall *wall = nullptr;
    Lever *lever = nullptr;
    setupWallCircuit(&grid, &wall, &lever);

    Engine engine;
    engine.processTick(&grid);

    QVERIFY(!wall->isMarkedForRemoval());   // 附着有效，不销毁
    QVERIFY(wall->isLit());
}

void test_RedstoneTorch::wall_missingAttachment_markedRemoval()
{
    GridModel grid;
    grid.resize(4, 3);

    // 无实心方块附着（背面 (1,0) 为空）
    auto wall = std::make_unique<RedstoneTorchWall>(1, 1);
    auto *wallPtr = wall.get();
    grid.placeComponent(1, 1, std::move(wall));

    Engine engine;
    engine.processTick(&grid);

    QVERIFY(wallPtr->isMarkedForRemoval());  // 悬空 → 标记销毁
    QCOMPARE(wallPtr->outputStrength(), 0);

    // 附着面为其他元件（非实心）同样销毁
    GridModel grid2;
    grid2.resize(4, 3);
    auto probe = std::make_unique<UniversalProbe>(1, 0, UniversalProbe::Role::Receiver);
    grid2.placeComponent(1, 0, std::move(probe));
    auto wall2 = std::make_unique<RedstoneTorchWall>(1, 1);
    auto *wall2Ptr = wall2.get();
    grid2.placeComponent(1, 1, std::move(wall2));

    engine.processTick(&grid2);
    QVERIFY(wall2Ptr->isMarkedForRemoval());
}

QTEST_APPLESS_MAIN(test_RedstoneTorch)

#include "test_RedstoneTorch.moc"
