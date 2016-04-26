#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDlg(QWidget *parent = 0);
    ~SettingsDlg();

    void loadSettings();
    void storeSettings();

private:
    Ui::SettingsDlg *ui;

    void populatePortLists();
};

#endif // SETTINGSDLG_H
