#include "selectoscsdlg.h"
#include "ui_selectoscsdlg.h"
#include <QListWidgetItem>



SelectOscsDlg::SelectOscsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectOscsDlg)
{
    ui->setupUi(this);
}

SelectOscsDlg::~SelectOscsDlg()
{
    delete ui;
}

void SelectOscsDlg::setOscillograms(QList<drive_event_id_t> oscs_list)
{
    ui->lwOscs->clear();

    for(auto it: oscs_list){
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(tr("Осциллограмма события №%1").arg(it));
        item->setCheckState(Qt::Unchecked);
        ui->lwOscs->addItem(item);
    }
}

QList<size_t> SelectOscsDlg::selectedOscillograms() const
{
    QList<size_t> res;

    for(int i = 0; i < ui->lwOscs->count(); i ++){
        if(ui->lwOscs->item(i)->checkState() == Qt::Checked){
            res.append(static_cast<size_t>(i));
        }
    }

    return res;
}

void SelectOscsDlg::on_pbSelectAll_clicked()
{
    for(int i = 0; i < ui->lwOscs->count(); i ++){
        ui->lwOscs->item(i)->setCheckState(Qt::Checked);
    }
}

void SelectOscsDlg::on_pbSelectNone_clicked()
{
    for(int i = 0; i < ui->lwOscs->count(); i ++){
        ui->lwOscs->item(i)->setCheckState(Qt::Unchecked);
    }
}

void SelectOscsDlg::on_pbSelectLast_clicked()
{
    if(ui->lwOscs->count() != 0){
        ui->lwOscs->item(ui->lwOscs->count() - 1)->setCheckState(Qt::Checked);
    }
}
