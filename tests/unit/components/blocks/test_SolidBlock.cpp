#include <QtTest>

#include "components/blocks/SolidBlock.h"
#include "components/test/UniversalProbe.h"
#include "components/transmission/RedstoneDustCross.h"
#include "components/transmission/RedstoneTorchGround.h"
#include "core/engine/Engine.h"
#include "core/model/GridModel.h"

/// 实心方块单元测试：不衰减传导、混合充能取 max、强弱充能规则、火把邻居过滤
class test_SolidBlock : public QObject
{
    Q_OBJECT

private slots:
    void solid_transmitsWithoutDecay();
    void solid_weakPower_cannotActivateDust();
    void solid_strongPower_activatesDust();
    void solid_ignoresTorchNeighbor();
    void solid_mixedPower_takesMax();
    void solid_interact_cyclesMaterial();
};

void test_SolidBlock::solid_transmitsWithoutDecay()
{
    GridModel grid;
    grid.resize(4, 3);

    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    source->setStrongOutputEnabled(true);
    grid.placeComponent(0, 1, std::move(source));

    auto block = std::make_unique<SolidBlock>(1, 1);
    auto *blockPtr = block.get();
    grid.placeComponent(1, 1, std::move(block));

    auto receiver = std::make_unique<UniversalProbe>(2, 1, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(2, 1, std::move(receiver));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(blockPtr->outputStrength(), 15);       // 不衰减传导
    QVERIFY(blockPtr->isStrongOutput());            // 被强充能 → 输出强充能
    QCOMPARE(recvPtr->receivedSignal(Direction::West).strength, 15);
}

void test_SolidBlock::solid_weakPower_cannotActivateDust()
{
    GridModel grid;
    grid.resize(4, 3);

    // 弱充能 15 注入方块
    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    grid.placeComponent(0, 1, std::move(source));

    auto block = std::make_unique<SolidBlock>(1, 1);
    grid.placeComponent(1, 1, std::move(block));

    auto dust = std::make_unique<RedstoneDustCross>(2, 1);
    auto *dustPtr = dust.get();
    grid.placeComponent(2, 1, std::move(dust));

    Engine engine;
    engine.processTick(&grid);

    // 核心规则：实心方块的弱充能不能激活红石粉
    QCOMPARE(dustPtr->outputStrength(), 0);
}

void test_SolidBlock::solid_strongPower_activatesDust()
{
    GridModel grid;
    grid.resize(4, 3);

    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    source->setStrongOutputEnabled(true);
    grid.placeComponent(0, 1, std::move(source));

    auto block = std::make_unique<SolidBlock>(1, 1);
    grid.placeComponent(1, 1, std::move(block));

    auto dust = std::make_unique<RedstoneDustCross>(2, 1);
    auto *dustPtr = dust.get();
    grid.placeComponent(2, 1, std::move(dust));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(dustPtr->outputStrength(), 14);   // 强充能 → 方块 → 粉激活（15-1）
}

void test_SolidBlock::solid_ignoresTorchNeighbor()
{
    GridModel grid;
    grid.resize(4, 3);

    // 火把在方块南边（输出 15）
    auto torch = std::make_unique<RedstoneTorchGround>(1, 2);
    grid.placeComponent(1, 2, std::move(torch));

    auto block = std::make_unique<SolidBlock>(1, 1);
    auto *blockPtr = block.get();
    grid.placeComponent(1, 1, std::move(block));

    Engine engine;
    engine.processTick(&grid);

    // 防自激/防循环充能：方块忽略火把邻居的信号
    QCOMPARE(blockPtr->outputStrength(), 0);
}

void test_SolidBlock::solid_mixedPower_takesMax()
{
    GridModel grid;
    grid.resize(4, 3);

    // 西侧强充能 7，东侧弱充能 15
    auto strong = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    strong->setSourceStrength(7);
    strong->setStrongOutputEnabled(true);
    grid.placeComponent(0, 1, std::move(strong));

    auto weak = std::make_unique<UniversalProbe>(2, 1, UniversalProbe::Role::Source);
    weak->setSourceStrength(15);
    grid.placeComponent(2, 1, std::move(weak));

    auto block = std::make_unique<SolidBlock>(1, 1);
    auto *blockPtr = block.get();
    grid.placeComponent(1, 1, std::move(block));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(blockPtr->outputStrength(), 15);    // 取 max（15 > 7）
    QVERIFY(blockPtr->isStrongOutput());         // 任一强充能 → 整体强充能
}

void test_SolidBlock::solid_interact_cyclesMaterial()
{
    SolidBlock block(0, 0);
    QVERIFY(block.isSolid());

    // 材质切换不影响信号行为，仅验证交互不破坏状态
    block.onInteract();
    block.onInteract();
    block.onInteract();
    block.onInteract();
    // 无崩溃即可（材质索引循环回绕）
    QVERIFY(true);
}

QTEST_APPLESS_MAIN(test_SolidBlock)

#include "test_SolidBlock.moc"
