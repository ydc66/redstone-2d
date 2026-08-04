#include <QtTest>

#include "components/sources/RedstoneBlock.h"
#include "components/test/UniversalProbe.h"
#include "core/engine/Engine.h"
#include "core/model/GridModel.h"

/// 红石块单元测试：恒定强充能 15 信号源
class test_RedstoneBlock : public QObject
{
    Q_OBJECT

private slots:
    void redstoneBlock_constantStrong15();
};

void test_RedstoneBlock::redstoneBlock_constantStrong15()
{
    GridModel grid;
    grid.resize(4, 3);

    auto block = std::make_unique<RedstoneBlock>(1, 1);
    auto *blockPtr = block.get();
    grid.placeComponent(1, 1, std::move(block));

    auto receiver = std::make_unique<UniversalProbe>(2, 1, UniversalProbe::Role::Receiver);
    auto *recvPtr = receiver.get();
    grid.placeComponent(2, 1, std::move(receiver));

    Engine engine;
    engine.processTick(&grid);

    QCOMPARE(blockPtr->outputStrength(), 15);   // 恒定 15
    QVERIFY(blockPtr->isStrongOutput());        // 强充能

    RedstoneSignal sig = recvPtr->receivedSignal(Direction::West);
    QCOMPARE(sig.strength, 15);
    QVERIFY(sig.isStrong);
}

QTEST_APPLESS_MAIN(test_RedstoneBlock)

#include "test_RedstoneBlock.moc"
