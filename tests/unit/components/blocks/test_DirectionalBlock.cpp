#include <QtTest>

#include "components/blocks/DirectionalBlock.h"

/// 方向箭头方块单元测试：纯装饰元件的朝向旋转交互
class test_DirectionalBlock : public QObject
{
    Q_OBJECT

private slots:
    void directional_initialFacingNorth();
    void directional_interact_rotatesClockwise();
};

void test_DirectionalBlock::directional_initialFacingNorth()
{
    DirectionalBlock block(0, 0);
    QCOMPARE(block.facing(), Direction::North);
    QVERIFY(!block.isSolid());              // NonSolid，不参与信号传播
}

void test_DirectionalBlock::directional_interact_rotatesClockwise()
{
    DirectionalBlock block(0, 0);

    block.onInteract();
    QCOMPARE(block.facing(), Direction::East);
    block.onInteract();
    QCOMPARE(block.facing(), Direction::South);
    block.onInteract();
    QCOMPARE(block.facing(), Direction::West);
    block.onInteract();
    QCOMPARE(block.facing(), Direction::North);   // 四步循环
}

QTEST_APPLESS_MAIN(test_DirectionalBlock)

#include "test_DirectionalBlock.moc"
