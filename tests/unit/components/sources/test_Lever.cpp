#include <QtTest>

#include "components/sources/Lever.h"
#include "components/test/UniversalProbe.h"
#include "core/engine/Engine.h"
#include "core/model/GridModel.h"

/// 拉杆单元测试：交互切换 ON/OFF、强充能 15 输出
class test_Lever : public QObject
{
    Q_OBJECT

private slots:
    void lever_initialOff();
    void lever_toggleOn_outputsStrong15();
    void lever_toggleOff_backToZero();
};

void test_Lever::lever_initialOff()
{
    GridModel grid;
    grid.resize(4, 3);

    auto lever = std::make_unique<Lever>(1, 1);
    auto *leverPtr = lever.get();
    grid.placeComponent(1, 1, std::move(lever));

    Engine engine;
    engine.processTick(&grid);

    QVERIFY(!leverPtr->isPowered());
    QCOMPARE(leverPtr->outputStrength(), 0);
}

void test_Lever::lever_toggleOn_outputsStrong15()
{
    GridModel grid;
    grid.resize(4, 3);

    auto lever = std::make_unique<Lever>(1, 1);
    auto *leverPtr = lever.get();
    grid.placeComponent(1, 1, std::move(lever));

    auto receiver = std::make_unique<UniversalProbe>(2, 1, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(2, 1, std::move(receiver));

    Engine engine;
    leverPtr->onInteract();          // 打开
    QVERIFY(leverPtr->isPowered());
    engine.processTick(&grid);

    QCOMPARE(leverPtr->outputStrength(), 15);
    QVERIFY(leverPtr->isStrongOutput());                       // 拉杆 = 强充能源
    RedstoneSignal sig = recvPtr->receivedSignal(Direction::West);
    QCOMPARE(sig.strength, 15);
    QVERIFY(sig.isStrong);
}

void test_Lever::lever_toggleOff_backToZero()
{
    GridModel grid;
    grid.resize(4, 3);

    auto lever = std::make_unique<Lever>(1, 1);
    auto *leverPtr = lever.get();
    grid.placeComponent(1, 1, std::move(lever));

    Engine engine;
    leverPtr->onInteract();          // 开
    engine.processTick(&grid);
    QCOMPARE(leverPtr->outputStrength(), 15);

    leverPtr->onInteract();          // 关
    QVERIFY(!leverPtr->isPowered());
    engine.processTick(&grid);
    QCOMPARE(leverPtr->outputStrength(), 0);
}

QTEST_APPLESS_MAIN(test_Lever)

#include "test_Lever.moc"
