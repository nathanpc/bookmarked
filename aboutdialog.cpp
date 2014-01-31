#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent),
											ui(new Ui::AboutDialog) {
	ui->setupUi(this);
}

AboutDialog::~AboutDialog() {
	delete ui;
}

/**
 * Sets the details in the about dialog.
 *
 * @param name Application name.
 * @param version Application version.
 */
void AboutDialog::setDetail(QString name, QString version) {
	ui->app_title->setText(name + " v" + version);
}
