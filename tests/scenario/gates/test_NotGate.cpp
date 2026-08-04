#include <QtTest>

#include "TestCircuit.h"

using namespace test_circuit;

/// 非门（反相器）场景测试
///
/// 布局（火把附着北面方块，粉朝北直连，1 列 5 格）：
///   拉杆 (1,1) → 方块 (1,2) → 火把 (1,3) → 粉 (1,4) → 灯 (1,5)
///
/// 行为：拉杆 ON → 方块强充能 → 火把背面被充能熄灭 → 灯灭；
///       拉杆 OFF → 火把点亮 → 粉衰减输出 14 → 灯亮。
class test_NotGate : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void leverOff_lampLit();
    void leverOn_lampOff();
    void leverOff_again_lampLit();

private:
    GridModel m_grid;
    Engine    m_engine;
};

void test_NotGate::init()
{
    m_grid.resize(3, 7);
    placeLever(m_grid, 1, 1);
    placeSolidBlock(m_grid, 1, 2);
    placeTorch(m_grid, 1, 3);     // 附着面 Back=North 指向 (1,2) 方块
    placeLine(m_grid, 1, 4);      // facing=North：北口接火把，南口出
    placeLamp(m_grid, 1, 5);
    runTicks(m_engine, m_grid);   // 初始稳态：拉杆 OFF → 火把亮 → 灯亮
}

void test_NotGate::leverOff_lampLit()
{
    QVERIFY(at<RedstoneLamp>(m_grid, 1, 5)->isLit());
    QVERIFY(at<RedstoneTorchWall>(m_grid, 1, 3)->isLit());
    QCOMPARE(at<Component>(m_grid, 1, 4)->outputStrength(), 14);   // 粉衰减 15-1
}

void test_NotGate::leverOn_lampOff()
{
    at<Lever>(m_grid, 1, 1)->onInteract();   // ON
    runTicks(m_engine, m_grid);

    QVERIFY(!at<RedstoneLamp>(m_grid, 1, 5)->isLit());
    QVERIFY(!at<RedstoneTorchWall>(m_grid, 1, 3)->isLit());
    QCOMPARE(at<Component>(m_grid, 1, 4)->outputStrength(), 0);
}

void test_NotGate::leverOff_again_lampLit()
{
    at<Lever>(m_grid, 1, 1)->onInteract();   // ON
    runTicks(m_engine, m_grid);
    at<Lever>(m_grid, 1, 1)->onInteract();   // OFF
    runTicks(m_engine, m_grid);

    QVERIFY(at<RedstoneLamp>(m_grid, 1, 5)->isLit());
    QVERIFY(at<RedstoneTorchWall>(m_grid, 1, 3)->isLit());
}

QTEST_GUILESS_MAIN(test_NotGate)
#include "test_NotGate.moc"
