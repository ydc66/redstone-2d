#include <QtTest>

#include "core/meta_component/Direction.h"

/// 方向系统单元测试：四方向取余模型的具体值表与代数不变量。
/// 与 Direction.h 文末 static_assert 契约组互补——编译期锁定值表，
/// 本测试在运行时覆盖全 16 组合并给出可读失败输出。
class test_Direction : public QObject
{
    Q_OBJECT

private slots:
    // ─── 具体值表 ───
    void opposite_values();
    void rotateRight_values();
    void rotateLeft_values();
    void dxdy_values();
    void allDirections_order();
    void enumIndex_contract();

    // ─── 代数不变量 ───
    void rotateTwice_isOpposite();
    void rotateFourTimes_isIdentity();
    void relativeDir_identity_isFront();
    void relDirOpposite_pairs();

    // ─── 双向映射（全 16 组合） ───
    void toRelativeDir_fullTable();
    void toAbsoluteDir_fullTable();
    void mapping_roundtrip();
};

void test_Direction::opposite_values()
{
    QCOMPARE(opposite(Direction::North), Direction::South);
    QCOMPARE(opposite(Direction::East),  Direction::West);
    QCOMPARE(opposite(Direction::South), Direction::North);
    QCOMPARE(opposite(Direction::West),  Direction::East);
    QCOMPARE(opposite(Direction::None),  Direction::None);
}

void test_Direction::rotateRight_values()
{
    QCOMPARE(rotateRight(Direction::North), Direction::East);
    QCOMPARE(rotateRight(Direction::East),  Direction::South);
    QCOMPARE(rotateRight(Direction::South), Direction::West);
    QCOMPARE(rotateRight(Direction::West),  Direction::North);
    QCOMPARE(rotateRight(Direction::None),  Direction::None);
}

void test_Direction::rotateLeft_values()
{
    QCOMPARE(rotateLeft(Direction::North), Direction::West);
    QCOMPARE(rotateLeft(Direction::West),  Direction::South);
    QCOMPARE(rotateLeft(Direction::South), Direction::East);
    QCOMPARE(rotateLeft(Direction::East),  Direction::North);
    QCOMPARE(rotateLeft(Direction::None),  Direction::None);
}

void test_Direction::dxdy_values()
{
    // 屏幕坐标 Y 向下：South 为 +1
    QCOMPARE(dx(Direction::North), 0);   QCOMPARE(dy(Direction::North), -1);
    QCOMPARE(dx(Direction::East),  1);   QCOMPARE(dy(Direction::East),   0);
    QCOMPARE(dx(Direction::South), 0);   QCOMPARE(dy(Direction::South),  1);
    QCOMPARE(dx(Direction::West),  -1);  QCOMPARE(dy(Direction::West),   0);
    // None 不产生位移
    QCOMPARE(dx(Direction::None), 0);    QCOMPARE(dy(Direction::None), 0);
}

void test_Direction::allDirections_order()
{
    // 顺序必须为顺时针 N→E→S→W（枚举遍历与取余共同的前提）
    const std::array<Direction, 4> expected = {
        Direction::North, Direction::East, Direction::South, Direction::West};
    const auto dirs = allDirections();
    for (int i = 0; i < 4; ++i)
        QCOMPARE(dirs[i], expected[i]);
}

void test_Direction::enumIndex_contract()
{
    // 运行时镜像头文件 static_assert：取余模型依赖索引顺序
    QCOMPARE(static_cast<int>(Direction::North), 0);
    QCOMPARE(static_cast<int>(Direction::East),  1);
    QCOMPARE(static_cast<int>(Direction::South), 2);
    QCOMPARE(static_cast<int>(Direction::West),  3);
    QCOMPARE(static_cast<int>(Direction::None),  4);
}

void test_Direction::rotateTwice_isOpposite()
{
    // 转两次 = 反向（左右旋各自成立）
    for (Direction d : allDirections()) {
        QCOMPARE(rotateRight(rotateRight(d)), opposite(d));
        QCOMPARE(rotateLeft(rotateLeft(d)),   opposite(d));
    }
}

void test_Direction::rotateFourTimes_isIdentity()
{
    // 转四次回到自身（旋转群闭合）
    for (Direction d : allDirections()) {
        QCOMPARE(rotateRight(rotateRight(rotateRight(rotateRight(d)))), d);
        QCOMPARE(rotateLeft(rotateLeft(rotateLeft(rotateLeft(d)))),     d);
    }
}

void test_Direction::relativeDir_identity_isFront()
{
    // 朝向自身的方向永远是 Front（恒等映射）
    for (Direction facing : allDirections())
        QCOMPARE(toRelativeDir(facing, facing), RelDir::Front);
}

void test_Direction::relDirOpposite_pairs()
{
    QCOMPARE(opposite(RelDir::Front), RelDir::Back);
    QCOMPARE(opposite(RelDir::Back),  RelDir::Front);
    QCOMPARE(opposite(RelDir::Right), RelDir::Left);
    QCOMPARE(opposite(RelDir::Left),  RelDir::Right);
}

void test_Direction::toRelativeDir_fullTable()
{
    // 全 16 组合：行 = facing，列 = d，值为相对侧
    const RelDir expected[4][4] = {
        //        N                E                S                W
        /* N */ {RelDir::Front,   RelDir::Right,   RelDir::Back,    RelDir::Left},
        /* E */ {RelDir::Left,    RelDir::Front,   RelDir::Right,   RelDir::Back},
        /* S */ {RelDir::Back,    RelDir::Left,    RelDir::Front,   RelDir::Right},
        /* W */ {RelDir::Right,   RelDir::Back,    RelDir::Left,    RelDir::Front},
    };
    const Direction dirs[4] = {Direction::North, Direction::East,
                               Direction::South, Direction::West};
    for (int f = 0; f < 4; ++f)
        for (int d = 0; d < 4; ++d)
            QCOMPARE(toRelativeDir(dirs[d], dirs[f]), expected[f][d]);
}

void test_Direction::toAbsoluteDir_fullTable()
{
    // 全 16 组合：行 = facing，列 = side，值为绝对方向
    const Direction expected[4][4] = {
        //        Front            Right            Back             Left
        /* N */ {Direction::North, Direction::East,  Direction::South, Direction::West},
        /* E */ {Direction::East,  Direction::South, Direction::West,  Direction::North},
        /* S */ {Direction::South, Direction::West,  Direction::North, Direction::East},
        /* W */ {Direction::West,  Direction::North, Direction::East,  Direction::South},
    };
    const Direction facings[4] = {Direction::North, Direction::East,
                                  Direction::South, Direction::West};
    const RelDir sides[4] = {RelDir::Front, RelDir::Right, RelDir::Back, RelDir::Left};
    for (int f = 0; f < 4; ++f)
        for (int s = 0; s < 4; ++s)
            QCOMPARE(toAbsoluteDir(sides[s], facings[f]), expected[f][s]);
}

void test_Direction::mapping_roundtrip()
{
    // 互逆：绝对 → 相对 → 绝对 还原（全 16 组合）
    for (Direction facing : allDirections())
        for (Direction d : allDirections())
            QCOMPARE(toAbsoluteDir(toRelativeDir(d, facing), facing), d);
}

QTEST_APPLESS_MAIN(test_Direction)

#include "test_Direction.moc"
