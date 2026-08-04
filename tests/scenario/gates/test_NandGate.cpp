#include <QtTest>

#include "TestCircuit.h"

using namespace test_circuit;

/// 与非门（NAND）场景测试
///
/// 布局（两路对称，火把附着北面方块，直接相邻十字粉）：
///   拉杆A (1,1) → 方块A (1,2) → 火把A (1,3)  ┐
///   拉杆B (3,1) → 方块B (3,2) → 火把B (3,3)  ┴→ 十字粉 (2,3) → 灯 (2,4)
///
/// 行为：火把亮 ⇔ 对应拉杆 OFF；灯亮 ⇔ 任一火把亮 = NAND(A, B)。
/// 火把对粉是弱充能直连（非实心方块来源），验证弱充能可激活红石粉规则。
class test_NandGate : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void bothOff_lampLit();    // NAND(0,0) = 1
    void aOn_lampLit();        // NAND(1,0) = 1
    void bothOn_lampOff();     // NAND(1,1) = 0

private:
    GridModel m_grid;
    Engine    m_engine;
};

void test_NandGate::init()
{
    m_grid.resize(5, 6);

    placeLever(m_grid, 1, 1);
    placeSolidBlock(m_grid, 1, 2);
    placeTorch(m_grid, 1, 3);     // 附着面 Back=North 指向 (1,2) 方块

    placeLever(m_grid, 3, 1);
    placeSolidBlock(m_grid, 3, 2);
    placeTorch(m_grid, 3, 3);

    placeCross(m_grid, 2, 3);     // 东西口分别接火把A/B
    placeLamp(m_grid, 2, 4);
    runTicks(m_engine, m_grid);   // 初始稳态：两拉杆 OFF → 两火把亮 → 灯亮
}

void test_NandGate::bothOff_lampLit()
{
    QVERIFY(at<RedstoneLamp>(m_grid, 2, 4)->isLit());
    QVERIFY(at<RedstoneTorchWall>(m_grid, 1, 3)->isLit());
    QVERIFY(at<RedstoneTorchWall>(m_grid, 3, 3)->isLit());
    QCOMPARE(at<Component>(m_grid, 2, 3)->outputStrength(), 14);   // 火把弱充能直连粉，衰减 15-1
}

void test_NandGate::aOn_lampLit()
{
    at<Lever>(m_grid, 1, 1)->onInteract();   // A ON
    runTicks(m_engine, m_grid);

    QVERIFY(!at<RedstoneTorchWall>(m_grid, 1, 3)->isLit());   // 火把A 被方块充能熄灭
    QVERIFY(at<RedstoneTorchWall>(m_grid, 3, 3)->isLit());    // 火把B 仍亮
    QVERIFY(at<RedstoneLamp>(m_grid, 2, 4)->isLit());         // 灯亮：NAND(1,0)=1
}

void test_NandGate::bothOn_lampOff()
{
    at<Lever>(m_grid, 1, 1)->onInteract();   // A ON
    at<Lever>(m_grid, 3, 1)->onInteract();   // B ON
    runTicks(m_engine, m_grid);

    QVERIFY(!at<RedstoneTorchWall>(m_grid, 1, 3)->isLit());
    QVERIFY(!at<RedstoneTorchWall>(m_grid, 3, 3)->isLit());
    QVERIFY(!at<RedstoneLamp>(m_grid, 2, 4)->isLit());         // 灯灭：NAND(1,1)=0
    QCOMPARE(at<Component>(m_grid, 2, 3)->outputStrength(), 0);
}

QTEST_GUILESS_MAIN(test_NandGate)
#include "test_NandGate.moc"
