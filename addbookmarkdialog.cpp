#include <QDebug>

#include "addbookmarkdialog.h"
#include "ui_addbookmarkdialog.h"

AddBookmarkDialog::AddBookmarkDialog(QWidget *parent) : QDialog(parent),
														ui(new Ui::AddBookmarkDialog) {
	ui->setupUi(this);
}

AddBookmarkDialog::~AddBookmarkDialog() {
	delete ui;
}

/**
 * The user has hit the OK button.
 */
void AddBookmarkDialog::accept() {
	int page = ui->page->value();
	QString description = ui->description->toPlainText();
	qDebug() << "Add bookmark:" << isbn << page << description;

	QSqlQuery query;
	query.prepare("INSERT INTO Bookmarks (isbn, page, description) "
					  "VALUES (:isbn, :page, :description)");

	query.bindValue(":isbn", isbn);
	query.bindValue(":page", page);
	query.bindValue(":description", description);

	query.exec();
	done(QDialog::Accepted);
}
