#ifndef EDITCANVAS_H
#define EDITCANVAS_H

#include <QWidget>

class QGraphicsView;
class QGraphicsScene;

/**
 * @class   EditCanvas
 * @brief   电路编辑画布
 *
 * 封装 QGraphicsView + QGraphicsScene，提供基础的 2D 渲染画布。
 * 单一职责：只做视图/场景的创建与配置，不处理交互逻辑。
 *
 * 当前骨架阶段只配置基础属性，网格线和元件渲染后续实现。
 */
class EditCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit EditCanvas(QWidget *parent = nullptr);

    QGraphicsView  *view()  const { return m_view; }
    QGraphicsScene *scene() const { return m_scene; }

private:
    void setupUI();

    QGraphicsView  *m_view;
    QGraphicsScene *m_scene;
};

#endif // EDITCANVAS_H
