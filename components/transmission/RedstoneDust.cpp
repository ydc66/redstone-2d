#include "RedstoneDust.h"

#include "core/model/GridModel.h"
#include "core/meta_component/Direction.h"

#include <algorithm>

RedstoneDust::RedstoneDust(int x, int y, Direction facing,
                           QList<RelDir> inputPorts,
                           QList<RelDir> outputPorts)
    : Component(x, y, facing,
                std::move(inputPorts),
                std::move(outputPorts))
{
}

void RedstoneDust::computeOutput(GridModel *grid)
{
    int maxInput = 0;

    for (Direction dir : {Direction::North, Direction::East,
                          Direction::South, Direction::West}) {
        // 跳过本元件不支持的端口方向
        if (!canInputFrom(dir))
            continue;

        RedstoneSignal sig = grid->signalFrom(x(), y(), dir);

        // 弱充能只有来自实心方块时才不可激活红石粉
        if (!sig.isStrong) {
            int nx = x() + dx(dir), ny = y() + dy(dir);
            if (!grid->isValid(nx, ny)) continue;
            auto *neighbor = grid->cellAt(nx, ny);
            if (neighbor && neighbor->isSolid())
                continue;   // 实心方块的弱充能 → 跳过
        }

        if (sig.strength > maxInput)
            maxInput = sig.strength;
    }
    setOutputStrength(std::max(0, maxInput - 1));
}
