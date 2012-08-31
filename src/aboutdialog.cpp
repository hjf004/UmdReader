#include "../include/aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->iconLabel->setPixmap(QPixmap(":/resource/pixmap/umdreader_128.png"));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
