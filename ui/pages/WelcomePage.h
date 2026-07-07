#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>

/**
 * @class   WelcomePage
 * @brief   欢迎页面，默认启动时显示
 */
class WelcomePage : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = nullptr);
};

#endif // WELCOMEPAGE_H
