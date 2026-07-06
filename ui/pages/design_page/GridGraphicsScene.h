#ifndef GRIDGRAPHICSSCENE_H
#define GRIDGRAPHICSSCENE_H

#include <QGraphicsScene>

class GridModel;

/**
 * @class   GridGraphicsScene
 * @brief   带网格背景的 QGraphicsScene
 *
 * 在 QGraphicsScene 的 drawBackground() 中绘制网格线 + 元件渲染。
 * 构造时必须传入 GridModel。
 */
class GridGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GridGraphicsScene(GridModel *grid, QObject *parent = nullptr);

    /// 网格尺寸（格子数）
    int gridWidth()  const { return m_gridWidth; }
    int gridHeight() const { return m_gridHeight; }

    /// 每格像素大小
    int cellSize() const { return m_cellSize; }

    GridModel* gridModel() const { return m_gridModel; }

public slots:
    /// 重新设置网格尺寸
    void setGridSize(int width, int height, int cellSize);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void drawComponents(QPainter *painter, const QRectF &rect);

    // 网格尺寸
    int        m_gridWidth  = 16;
    int        m_gridHeight = 16;

    /// 每格像素大小（64px 更适合观察）
    int        m_cellSize   = 64;     
    GridModel *m_gridModel  = nullptr;

    /// 更新场景矩形
    void updateSceneRect();
};

#endif // GRIDGRAPHICSSCENE_H
