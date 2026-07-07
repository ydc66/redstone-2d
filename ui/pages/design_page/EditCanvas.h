#ifndef EDITCANVAS_H
#define EDITCANVAS_H

#include <QWidget>

class QGraphicsView;
class GridGraphicsScene;
class GridModel;

/**
 * @class   EditCanvas
 * @brief   电路编辑画布
 */
class EditCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit EditCanvas(GridModel *model, QWidget *parent = nullptr);

    QGraphicsView    *view()  const { return m_view; }
    GridGraphicsScene *scene() const { return m_scene; }

private:
    void setupUI(GridModel *model);

    QGraphicsView     *m_view;
    GridGraphicsScene *m_scene;
};

#endif // EDITCANVAS_H
