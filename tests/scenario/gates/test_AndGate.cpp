#include <QtTest>

#include "TestCircuit.h"

using namespace test_circuit;

/// 与门（AND）场景测试 = NAND + 非门（火把反相）
///
/// 布局：
///   NAND 部分同 test_NandGate（十字粉 (2,3) 为 NAND 输出）：
///     拉杆A (1,1) → 方块A (1,2) → 火把A (1,3)
///     拉杆B (3,1) → 方块B (3,2) → 火把B (3,3)
///   NAND 输出 → 非门反相：
///     十字粉 (2,3) → 粉 (2,4) → 方块 (2,5) → 火把 (2,6) → 灯 (2,7)
///
/// 行为：灯亮 = NOT(NAND(A,B)) = AND(A, B)。
class test_AndGate : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void bothOff_lampOff();    // AND(0,0) = 0
    void aOn_lampOff();        // AND(1,0) = 0
    void bothOn_lampOn();      // AND(1,1) = 1

private:
    GridModel m_grid;
    Engine    m_engine;
};

void test_AndGate::init()
{
    m_grid.resize(5, 9);

    // ─── NAND 部分 ───
    placeLever(m_grid, 1, 1);
    placeSolidBlock(m_grid, 1, 2);
    placeTorch(m_grid, 1, 3);

    placeLever(m_grid, 3, 1);
    placeSolidBlock(m_grid, 3, 2);
    placeTorch(m_grid, 3, 3);

    placeCross(m_grid, 2, 3);

    // ─── 反相部分 ───
    placeLine(m_grid, 2, 4);      // facing=North：北口接十字粉，南口出
    placeSolidBlock(m_grid, 2, 5);
    placeTorch(m_grid, 2, 6);     // 附着 (2,5) 方块
    placeLamp(m_grid, 2, 7);
    runTicks(m_engine, m_grid);   // 初始稳态：AND(0,0) → NAND=1 → 反相火把熄灭 → 灯灭
}

void test_AndGate::bothOff_lampOff()
{
    QVERIFY(!at<RedstoneLamp>(m_grid, 2, 7)->isLit());
    QVERIFY(!at<RedstoneTorchWall>(m_grid, 2, 6)->isLit());      // 反相火把被充能熄灭
    QCOMPARE(at<Component>(m_grid, 2, 3)->outputStrength(), 14); // NAND 输出（火把 15 直连衰减）
    QCOMPARE(at<Component>(m_grid, 2, 4)->outputStrength(), 13); // 反相链粉再衰减
}

void test_AndGate::aOn_lampOff()
{
    at<Lever>(m_grid, 1, 1)->onInteract();   // A ON
    runTicks(m_engine, m_grid);

    QVERIFY(!at<RedstoneLamp>(m_grid, 2, 7)->isLit());   // AND(1,0)=0
}

void test_AndGate::bothOn_lampOn()
{
    at<Lever>(m_grid, 1, 1)->onInteract();   // A ON
    at<Lever>(m_grid, 3, 1)->onInteract();   // B ON
    runTicks(m_engine, m_grid);

    QVERIFY(at<RedstoneLamp>(m_grid, 2, 7)->isLit());            // AND(1,1)=1
    QVERIFY(at<RedstoneTorchWall>(m_grid, 2, 6)->isLit());        // NAND=0 → 反相火把点亮
    QCOMPARE(at<Component>(m_grid, 2, 3)->outputStrength(), 0);
}

QTEST_GUILESS_MAIN(test_AndGate)
#include "test_AndGate.moc"
