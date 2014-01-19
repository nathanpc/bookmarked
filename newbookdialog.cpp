#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>

#include "newbookdialog.h"
#include "ui_newbookdialog.h"

NewBookDialog::NewBookDialog(QWidget *parent) : QDialog(parent),
												ui(new Ui::NewBookDialog) {
	ui->setupUi(this);

	got_cover = false;
	got_info = false;
}

NewBookDialog::~NewBookDialog() {
	delete ui;
}

void NewBookDialog::setISBNDBKey(QString key) {
	isbndb_key = key;
}

/**
 * Grabs the book cover image from the internet.
 *
 * @param isbn Book's ISBN-10.
 */
void NewBookDialog::grabBookCover(QString isbn) {
	// Setup the connection.
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QUrl url = QUrl("http://images.amazon.com/images/P/" + isbn + ".01.ZTZZZZZZ.jpg");
	book_cover_reply = manager->get(QNetworkRequest(url));

	// Setup the signals and slots.
	connect(book_cover_reply, SIGNAL(finished()), this, SLOT(on_book_cover_Finished()));
}

/**
 * Grabs more information about the book.
 *
 * @param isbn Book's ISBN.
 */
void NewBookDialog::grabBookInformation(QString isbn) {
	// Setup the connection.
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QUrl url = QUrl("http://isbndb.com/api/v2/json/" + isbndb_key + "/book/" + isbn);
	book_info_reply = manager->get(QNetworkRequest(url));

	// Setup the signals and slots.
	connect(book_info_reply, SIGNAL(finished()), this, SLOT(on_book_info_Finished()));
}

/**
 * Checks if everything is done and the data can be added to the database and close the dialog.
 */
void NewBookDialog::everythingDone() {
	if (got_cover && got_info) {
		QSqlQuery query;
		query.prepare("INSERT INTO Books (isbn, title, authors, cover) "
						  "VALUES (:isbn, :title, :authors, :cover)");

		query.bindValue(":isbn", info["isbn"]);
		query.bindValue(":title", info["title"]);
		query.bindValue(":authors", info["authors"]);
		query.bindValue(":cover", cover);

		query.exec();
		done(QDialog::Accepted);
	}
}

void NewBookDialog::accept() {
	QString isbn = ui->isbn->text();

	if (!isbn.isEmpty()) {
		grabBookCover(isbn);
		grabBookInformation(isbn);
	} else {
		QMessageBox::critical(this, "Error", "You must specify a ISBN.");
	}
}

void NewBookDialog::on_book_cover_Finished() {
	// Grab the image data.
	cover = book_cover_reply->readAll();

	//QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data));
	// Insert the image into the list item.
	//ui->books->item(row)->setData(Qt::DecorationRole, pixmap);

	qDebug() << "Got the image data";

	got_cover = true;
	everythingDone();
}

void NewBookDialog::on_book_info_Finished() {
	// Grab the info data.
	QByteArray data = book_info_reply->readAll();
	qDebug() << "Got the info data";

	// Parse the document.
	QJsonParseError err;
	QJsonObject json = QJsonDocument::fromJson(data, &err).object();

	// TODO: Check for error.

	// Get the important root.
	QJsonObject info = json.value("data").toArray()[0].toObject();
	QJsonArray authors_arr = info.value("author_data").toArray();

	// Get the authors.
	QString authors;
	for (int i = 0; i < authors_arr.size(); ++i) {
		if (i > 0) {
			authors += ", ";
		}

		authors += authors_arr[i].toObject().value("name").toString();
	}

	// Get ISBN-10 and title.
	QString isbn = info.value("isbn10").toString();
	QString title = info.value("title").toString();

	qDebug() << "Parsed the data:" << isbn << title << authors;

	// Populate the global info.
	this->info["isbn"]    = isbn;
	this->info["title"]   = title;
	this->info["authors"] = authors;

	// Finish.
	got_info = true;
	everythingDone();
}
