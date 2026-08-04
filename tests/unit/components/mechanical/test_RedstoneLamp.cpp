#include <QtTest>

#include "components/mechanical/RedstoneLamp.h"
#include "components/test/UniversalProbe.h"
#include "core/engine/Engine.h"
#include "core/model/GridModel.h"

/// 红石灯单元测试：消费者响应（有信号亮、无信号灭）
class test_RedstoneLamp : public QObject
{
    Q_OBJECT

private slots:
    void lamp_unpowered_off();
    void lamp_powered_lit();
    void lamp_powerLost_unlit();
};

void test_RedstoneLamp::lamp_unpowered_off()
{
    GridModel grid;
    grid.resize(4, 3);

    auto lamp = std::make_unique<RedstoneLamp>(1, 1);
    auto *lampPtr = lamp.get();
    grid.placeComponent(1, 1, std::move(lamp));

    Engine engine;
    engine.processTick(&grid);

    QVERIFY(!lampPtr->isLit());   // 无信号 → 灭
}

void test_RedstoneLamp::lamp_powered_lit()
{
    GridModel grid;
    grid.resize(4, 3);

    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    grid.placeComponent(0, 1, std::move(source));

    auto lamp = std::make_unique<RedstoneLamp>(1, 1);
    auto *lampPtr = lamp.get();
    grid.placeComponent(1, 1, std::move(lamp));

    Engine engine;
    engine.processTick(&grid);

    QVERIFY(lampPtr->isLit());   // 收到信号 → 亮
}

void test_RedstoneLamp::lamp_powerLost_unlit()
{
    GridModel grid;
    grid.resize(4, 3);

    auto source = std::make_unique<UniversalProbe>(0, 1, UniversalProbe::Role::Source);
    source->setSourceStrength(15);
    auto *srcPtr = source.get();
    grid.placeComponent(0, 1, std::move(source));

    auto lamp = std::make_unique<RedstoneLamp>(1, 1);
    auto *lampPtr = lamp.get();
    grid.placeComponent(1, 1, std::move(lamp));

    Engine engine;
    engine.processTick(&grid);
    QVERIFY(lampPtr->isLit());

    srcPtr->setSourceStrength(0);   // 源断电
    engine.processTick(&grid);
    QVERIFY(!lampPtr->isLit());     // 灯立即熄灭（无延迟）
}

QTEST_APPLESS_MAIN(test_RedstoneLamp)

#include "test_RedstoneLamp.moc"
