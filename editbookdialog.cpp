#include <QDebug>
#include <QSqlQuery>

#include "editbookdialog.h"
#include "ui_editbookdialog.h"

EditBookDialog::EditBookDialog(QWidget *parent) : QDialog(parent),
												  ui(new Ui::EditBookDialog) {
	ui->setupUi(this);
}

EditBookDialog::~EditBookDialog() {
	delete ui;
}

/**
 * Populate the edits in the view.
 *
 * @param title Book title.
 * @param authors Book authors.
 */
void EditBookDialog::populateEdits(QString title, QString authors) {
	ui->title->setText(title);
	ui->authors->setText(authors);
}

/**
 * Sets the ISBN.
 *
 * @param isbn Book's ISBN.
 */
void EditBookDialog::setISBN(QString isbn) {
	this->isbn = isbn;
}

/**
 * Update the details in the database.
 */
void EditBookDialog::on_buttonBox_accepted() {
	QString title = ui->title->text();
	QString authors = ui->authors->text();
	qDebug() << "Editing Book:" << isbn << title << authors;

	QSqlQuery query;
	query.prepare("UPDATE Books SET title = :title, authors = :authors "
				  "WHERE isbn=:isbn");

	query.bindValue(":isbn", isbn);
	query.bindValue(":title", title);
	query.bindValue(":authors", authors);

	query.exec();
	done(QDialog::Accepted);
}
