#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

/**
 * @class   SettingsPage
 * @brief   设置页面
 */
class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
};

#endif // SETTINGSPAGE_H
