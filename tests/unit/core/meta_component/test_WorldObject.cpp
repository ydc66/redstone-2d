#include <QtTest>

#include "core/meta_component/WorldObject.h"

/// 测试桩子类：WorldObject 的 paintContent 为纯虚，需具体子类才能实例化
class TestRenderObject : public WorldObject
{
public:
    TestRenderObject(int x, int y, Direction facing = Direction::North)
        : WorldObject(x, y, facing)
    {
    }

protected:
    void paintContent(QPainter *, int) const override {}
};

/// WorldObject 基类单元测试：网格占位、朝向旋转（渲染测试后续再补）
class test_WorldObject : public QObject
{
    Q_OBJECT

private slots:
    // ─── 网格占位 ───
    void initialPosition();
    void setPosition_updates();

    // ─── 朝向 / 旋转 ───
    void defaultFacing_isNorth();
    void setFacing_updates();
    void directionToAngle_values();
};

void test_WorldObject::initialPosition()
{
    WorldObject *obj = new TestRenderObject(3, 5);
    QCOMPARE(obj->x(), 3);
    QCOMPARE(obj->y(), 5);
    delete obj;
}

void test_WorldObject::setPosition_updates()
{
    TestRenderObject obj(0, 0);
    obj.setPosition(7, 9);
    QCOMPARE(obj.x(), 7);
    QCOMPARE(obj.y(), 9);
}

void test_WorldObject::defaultFacing_isNorth()
{
    TestRenderObject obj(0, 0);
    QCOMPARE(obj.facing(), Direction::North);
}

void test_WorldObject::setFacing_updates()
{
    TestRenderObject obj(0, 0, Direction::West);
    QCOMPARE(obj.facing(), Direction::West);

    obj.setFacing(Direction::East);
    QCOMPARE(obj.facing(), Direction::East);
}

void test_WorldObject::directionToAngle_values()
{
    QCOMPARE(WorldObject::directionToAngle(Direction::North), 0);
    QCOMPARE(WorldObject::directionToAngle(Direction::East),  90);
    QCOMPARE(WorldObject::directionToAngle(Direction::South), 180);
    QCOMPARE(WorldObject::directionToAngle(Direction::West),  270);
}

QTEST_APPLESS_MAIN(test_WorldObject)

#include "test_WorldObject.moc"
