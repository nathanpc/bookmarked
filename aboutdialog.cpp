#include <QDebug>

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent),
											ui(new Ui::AboutDialog) {
	ui->setupUi(this);
	ui->app_title->setText(QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion());
}

AboutDialog::~AboutDialog() {
	delete ui;
}
