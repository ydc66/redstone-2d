#ifndef DESIGNPAGE_H
#define DESIGNPAGE_H

#include <QWidget>

/**
 * @class   DesignPage
 * @brief   设计页面，电路编辑主区域
 */
class DesignPage : public QWidget
{
    Q_OBJECT

public:
    explicit DesignPage(QWidget *parent = nullptr);
};

#endif // DESIGNPAGE_H
