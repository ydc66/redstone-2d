#include <QtTest>

#include "TestCircuit.h"

using namespace test_circuit;

/// 或门场景测试
///
/// 布局（两拉杆 → 两根一字粉的输出端直接对准十字粉东西两口 → 灯）：
///   拉杆A (0,3) → 粉A (1,3) [facing=East，西口入、东口出]  ┐
///   拉杆B (4,3) → 粉B (3,3) [facing=West，东口入、西口出]  ┴→ 十字粉 (2,3) → 灯 (2,4)
///
/// 行为：任一拉杆 ON → 对应粉衰减输出 14 → 十字粉聚合 max 再衰减 13 → 灯亮。
/// 关键：信号只传给相邻格，粉的输出端必须与十字粉直接相邻且端口对准。
class test_OrGate : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void bothOff_lampOff();
    void aOn_lampOn();
    void bOn_lampOn();
    void bothOn_lampOn();

private:
    GridModel m_grid;
    Engine    m_engine;
};

void test_OrGate::init()
{
    m_grid.resize(5, 6);

    placeLever(m_grid, 0, 3);
    placeLineEast(m_grid, 1, 3);  // facing=East：西口入（接拉杆A），东口出 → 十字粉

    placeLever(m_grid, 4, 3);
    placeLineWest(m_grid, 3, 3);  // facing=West：东口入（接拉杆B），西口出 → 十字粉

    placeCross(m_grid, 2, 3);     // 东西两口直接接收两根粉的输出
    placeLamp(m_grid, 2, 4);
    runTicks(m_engine, m_grid);   // 初始稳态：两拉杆 OFF → 灯灭
}

void test_OrGate::bothOff_lampOff()
{
    QVERIFY(!at<RedstoneLamp>(m_grid, 2, 4)->isLit());
    QCOMPARE(at<Component>(m_grid, 2, 3)->outputStrength(), 0);
}

void test_OrGate::aOn_lampOn()
{
    at<Lever>(m_grid, 0, 3)->onInteract();   // A ON
    runTicks(m_engine, m_grid);

    QVERIFY(at<RedstoneLamp>(m_grid, 2, 4)->isLit());
    QCOMPARE(at<Component>(m_grid, 1, 3)->outputStrength(), 14);   // 粉A 衰减 15-1
    QCOMPARE(at<Component>(m_grid, 2, 3)->outputStrength(), 13);   // 十字粉再衰减
}

void test_OrGate::bOn_lampOn()
{
    at<Lever>(m_grid, 4, 3)->onInteract();   // B ON
    runTicks(m_engine, m_grid);

    QVERIFY(at<RedstoneLamp>(m_grid, 2, 4)->isLit());
    QCOMPARE(at<Component>(m_grid, 3, 3)->outputStrength(), 14);   // 粉B 衰减 15-1
    QCOMPARE(at<Component>(m_grid, 2, 3)->outputStrength(), 13);   // 十字粉再衰减
}

void test_OrGate::bothOn_lampOn()
{
    at<Lever>(m_grid, 0, 3)->onInteract();   // A ON
    at<Lever>(m_grid, 4, 3)->onInteract();   // B ON
    runTicks(m_engine, m_grid);

    QVERIFY(at<RedstoneLamp>(m_grid, 2, 4)->isLit());
    QCOMPARE(at<Component>(m_grid, 2, 3)->outputStrength(), 13);   // 十字粉 max(14,14)-1
}

QTEST_GUILESS_MAIN(test_OrGate)
#include "test_OrGate.moc"
