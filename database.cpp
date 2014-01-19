#include <QMessageBox>

#include "database.h"

Database::Database(QObject *parent) : QObject(parent) {
}

/**
 * Opens a connection to thedatabase.
 *
 * @return Status of the connection. False indicates it couldn't open.
 */
bool Database::open() {
	// Setup the connection.
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("/home/nathanpc/Developer/Qt/Bookmarked/bookmarked.db"); // TODO: Use a common path.

	// Open the connection to the database.
	return db.open();
}

/**
 * Get the books from the database and store it in Database::books.
 *
 * @return Number of rows returned.
 */
int Database::getBooks() {
	// Clear the cache.
	books.clear();

	// Query the database.
	QSqlQuery query;
	query.exec("SELECT * FROM Books");

	while (query.next()) {
		QString isbn  = query.value(0).toString();
		QString title = query.value(1).toString();

		QHash<QString, QString> book;
		book["isbn"] = isbn;
		book["title"] = title;

		books << book;
		qDebug() << "Book:" << isbn << title;
	}

	return query.size();
}

/**
 * Get the bookmarks from the database and store it in Database::bookmarks.
 *
 * @return Number of rows returned.
 */
int Database::getBookmarks() {
	// Clear the cache.
	bookmarks.clear();

	// Query the database.
	QSqlQuery query;
	query.exec("SELECT * FROM Bookmarks");

	while (query.next()) {
		QString isbn = query.value(0).toString();
		QString page = query.value(1).toString();
		QString description = query.value(2).toString();

		QHash<QString, QString> bookmark;
		bookmark["isbn"] = isbn;
		bookmark["page"] = page;
		bookmark["description"] = description;

		bookmarks << bookmark;
		qDebug() << "Bookmark:" << isbn << page << description;
	}

	return query.size();
}
