#include <QtTest>

#include "components/test/UniversalProbe.h"
#include "components/transmission/RedstoneDustCross.h"
#include "components/transmission/RedstoneDustLine.h"
#include "core/engine/Engine.h"
#include "core/model/GridModel.h"

/// 红石粉家族单元测试：衰减传播、端口方向性、弱充能过滤（Cross 四向 / Line 两端）
class test_RedstoneDust : public QObject
{
    Q_OBJECT

private slots:
    // ─── 衰减逻辑（RedstoneDust 基类） ───
    void cross_decayByOne();
    void cross_chainDecay();
    void cross_strengthOne_notNegative();
    void cross_weakNonSolid_activates();

    // ─── Line 端口方向性 ───
    void line_onlyEndsReceive();
    void line_rotate_swapsPorts();
    void line_interact_rotatesClockwise();
};

void test_RedstoneDust::cross_decayByOne()
{
    GridModel grid;
    grid.resize(4, 3);

    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    grid.placeComponent(0, 1, std::move(source));

    auto dust = std::make_unique<RedstoneDustCross>(1, 1);
    auto *dustPtr = dust.get();
    grid.placeComponent(1, 1, std::move(dust));

    auto receiver = std::make_unique<UniversalProbe>(2, 1, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(2, 1, std::move(receiver));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(dustPtr->outputStrength(), 14);               // max-1 衰减
    QCOMPARE(recvPtr->receivedSignal(Direction::West).strength, 14);
}

void test_RedstoneDust::cross_chainDecay()
{
    GridModel grid;
    grid.resize(4, 3);

    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    grid.placeComponent(0, 1, std::move(source));

    auto dust1 = std::make_unique<RedstoneDustCross>(1, 1);
    auto *dust1Ptr = dust1.get();
    grid.placeComponent(1, 1, std::move(dust1));

    auto dust2 = std::make_unique<RedstoneDustCross>(2, 1);
    auto *dust2Ptr = dust2.get();
    grid.placeComponent(2, 1, std::move(dust2));

    auto receiver = std::make_unique<UniversalProbe>(3, 1, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(3, 1, std::move(receiver));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(dust1Ptr->outputStrength(), 14);   // 15 → 14
    QCOMPARE(dust2Ptr->outputStrength(), 13);   // 14 → 13（每格衰减 1）
    QCOMPARE(recvPtr->receivedSignal(Direction::West).strength, 13);
}

void test_RedstoneDust::cross_strengthOne_notNegative()
{
    GridModel grid;
    grid.resize(4, 3);

    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(1);
    grid.placeComponent(0, 1, std::move(source));

    auto dust = std::make_unique<RedstoneDustCross>(1, 1);
    auto *dustPtr = dust.get();
    grid.placeComponent(1, 1, std::move(dust));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(dustPtr->outputStrength(), 0);   // max(0, 1-1)，不出现负数
}

void test_RedstoneDust::cross_weakNonSolid_activates()
{
    GridModel grid;
    grid.resize(4, 3);

    // 弱充能 7（非实心方块来源，如火把/粉自身）：可以激活红石粉
    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(7);
    grid.placeComponent(0, 1, std::move(source));

    auto dust = std::make_unique<RedstoneDustCross>(1, 1);
    auto *dustPtr = dust.get();
    grid.placeComponent(1, 1, std::move(dust));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(dustPtr->outputStrength(), 6);   // 7 → 6，弱充能有效
}

void test_RedstoneDust::line_onlyEndsReceive()
{
    GridModel grid;
    grid.resize(4, 3);

    // facing=North 的竖线：端口仅 North/South
    auto line = std::make_unique<RedstoneDustLine>(1, 1);
    auto *linePtr = line.get();
    grid.placeComponent(1, 1, std::move(line));

    // 侧面（West）放源：非端口方向，不接收
    auto side = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    side->setSourceStrength(15);
    grid.placeComponent(0, 1, std::move(side));

    Engine engine;
    engine.processTick(&grid);
    QCOMPARE(linePtr->outputStrength(), 0);   // 侧面信号被端口过滤

    // 端部（North）放源：端口方向，正常衰减接收
    grid.removeComponentAt(0, 1);
    auto end = std::make_unique<UniversalProbe>(1, 0, UniversalProbe::Role::Source);
    end->setSourceStrength(15);
    grid.placeComponent(1, 0, std::move(end));

    engine.processTick(&grid);
    QCOMPARE(linePtr->outputStrength(), 14);   // 15 → 14
}

void test_RedstoneDust::line_rotate_swapsPorts()
{
    GridModel grid;
    grid.resize(4, 3);

    auto line = std::make_unique<RedstoneDustLine>(1, 1);
    auto *linePtr = line.get();
    grid.placeComponent(1, 1, std::move(line));
    linePtr->onInteract();                       // 旋转 → facing East（横线，端口 East/West）

    auto side = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    side->setSourceStrength(15);
    grid.placeComponent(0, 1, std::move(side));

    Engine engine;
    engine.processTick(&grid);
    QCOMPARE(linePtr->outputStrength(), 14);   // 旋转后 West 变为端口

    // 原端口方向（North）现在被过滤
    grid.removeComponentAt(0, 1);
    auto end = std::make_unique<UniversalProbe>(1, 0, UniversalProbe::Role::Source);
    end->setSourceStrength(15);
    grid.placeComponent(1, 0, std::move(end));

    engine.processTick(&grid);
    QCOMPARE(linePtr->outputStrength(), 0);
}

void test_RedstoneDust::line_interact_rotatesClockwise()
{
    RedstoneDustLine line(0, 0);
    QCOMPARE(line.facing(), Direction::North);

    line.onInteract();
    QCOMPARE(line.facing(), Direction::East);
    line.onInteract();
    QCOMPARE(line.facing(), Direction::South);
    line.onInteract();
    QCOMPARE(line.facing(), Direction::West);
    line.onInteract();
    QCOMPARE(line.facing(), Direction::North);   // 四步循环
}

QTEST_APPLESS_MAIN(test_RedstoneDust)

#include "test_RedstoneDust.moc"
