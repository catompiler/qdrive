#ifndef SELECTOSCSDLG_H
#define SELECTOSCSDLG_H

#include <QDialog>
#include <QList>
#include "drive_types.h"

namespace Ui {
class SelectOscsDlg;
}

class SelectOscsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectOscsDlg(QWidget *parent = 0);
    ~SelectOscsDlg();

    /**
     * @brief Устанавливает список осциллограмм.
     * @param oscs_list Список осциллограмм.
     */
    void setOscillograms(QList<drive_event_id_t> oscs_list);

    /**
     * @brief Получает список индексов
     * выбранных осциллограмм.
     * @return Список индексов выбранных осциллограмм.
     */
    QList<size_t> selectedOscillograms() const;

private slots:
    void on_pbSelectAll_clicked();
    void on_pbSelectNone_clicked();
    void on_pbSelectLast_clicked();

private:
    Ui::SelectOscsDlg *ui;
};

#endif // SELECTOSCSDLG_H
