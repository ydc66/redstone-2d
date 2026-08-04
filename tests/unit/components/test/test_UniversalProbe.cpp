#include <QtTest>

#include "components/test/UniversalProbe.h"
#include "core/engine/Engine.h"
#include "core/model/GridModel.h"

/// UniversalProbe 万能探针单元测试：准确信号源、按方向取信号
class test_UniversalProbe : public QObject
{
    Q_OBJECT

private slots:
    // ─── Source 角色：准确信号源 ───
    void source_outputsConfiguredStrength();
    void source_weakAndStrongFlag();

    // ─── Receiver 角色：按方向取信号 ───
    void receiver_readsDirectionalSignal();
    void receiver_isolated_readsZero();

    // ─── Both 角色 ───
    void both_role_simultaneous();
};

void test_UniversalProbe::source_outputsConfiguredStrength()
{
    GridModel grid;
    grid.resize(3, 3);

    auto source = std::make_unique<UniversalProbe>(1, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    auto *srcPtr = source.get();
    grid.placeComponent(1, 1, std::move(source));

    auto receiver = std::make_unique<UniversalProbe>(1, 2, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(1, 2, std::move(receiver));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(srcPtr->outputStrength(), 15);   // 源输出 = 配置强度
    // 按方向查询：源在接收探针的北边
    QCOMPARE(recvPtr->receivedSignal(Direction::North).strength, 15);
    QCOMPARE(recvPtr->receivedSignal(Direction::South).strength, 0);   // 南边无信号
}

void test_UniversalProbe::source_weakAndStrongFlag()
{
    GridModel grid;
    grid.resize(3, 3);

    auto source = std::make_unique<UniversalProbe>(1, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(7);
    source->setStrongOutputEnabled(true);
    grid.placeComponent(1, 1, std::move(source));

    auto receiver = std::make_unique<UniversalProbe>(1, 2, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(1, 2, std::move(receiver));

    Engine engine;
    engine.processTick(&grid);

    // 接收探针 (1,2) 从北邻居收到：强度 7 且为强充能
    RedstoneSignal sig = recvPtr->receivedSignal(Direction::North);
    QCOMPARE(sig.strength, 7);
    QVERIFY(sig.isStrong);

    // 弱充能切换
    auto source2 = std::make_unique<UniversalProbe>(1, 1, UniversalProbe::Role::Source);
    source2->setSourceStrength(7);
    auto *src2Ptr = source2.get();
    grid.placeComponent(1, 1, std::move(source2));

    engine.processTick(&grid);
    QCOMPARE(recvPtr->receivedSignal(Direction::North).isStrong, false);
    QCOMPARE(src2Ptr->isStrongOutput(), false);
}

void test_UniversalProbe::receiver_readsDirectionalSignal()
{
    GridModel grid;
    grid.resize(3, 3);

    auto source = std::make_unique<UniversalProbe>(1, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    auto *srcPtr = source.get();
    grid.placeComponent(1, 1, std::move(source));

    auto receiver = std::make_unique<UniversalProbe>(1, 2, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(1, 2, std::move(receiver));

    Engine engine;
    engine.processTick(&grid);

    // 强度 15 → 7：第二次 tick 后方向查询反映最新值
    srcPtr->setSourceStrength(7);
    engine.processTick(&grid);
    QCOMPARE(recvPtr->receivedSignal(Direction::North).strength, 7);

    // 三面（东/南/西）均无信号
    QCOMPARE(recvPtr->receivedSignal(Direction::East).strength, 0);
    QCOMPARE(recvPtr->receivedSignal(Direction::South).strength, 0);
    QCOMPARE(recvPtr->receivedSignal(Direction::West).strength, 0);
}

void test_UniversalProbe::receiver_isolated_readsZero()
{
    GridModel grid;
    grid.resize(3, 3);

    auto receiver = std::make_unique<UniversalProbe>(1, 1, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(1, 1, std::move(receiver));

    Engine engine;
    engine.processTick(&grid);

    // 无信号环境：四方向均为空信号（强度 0）
    QCOMPARE(recvPtr->receivedSignal(Direction::North).strength, 0);
    QCOMPARE(recvPtr->receivedSignal(Direction::West).strength, 0);
}

void test_UniversalProbe::both_role_simultaneous()
{
    GridModel grid;
    grid.resize(3, 3);

    // 中间节点：既输出 5，又接收南邻居强源
    auto both = std::make_unique<UniversalProbe>(1, 1, UniversalProbe::Role::Both);
    both->setSourceStrength(5);
    auto *bothPtr = both.get();
    grid.placeComponent(1, 1, std::move(both));

    auto source = std::make_unique<UniversalProbe>(1, 2, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    grid.placeComponent(1, 2, std::move(source));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(bothPtr->outputStrength(), 5);   // 源侧输出保持配置
    QCOMPARE(bothPtr->receivedSignal(Direction::South).strength, 15);   // 强源在南方
}

QTEST_APPLESS_MAIN(test_UniversalProbe)

#include "test_UniversalProbe.moc"
