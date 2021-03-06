#include <QMessageBox>
#include <QDesktopServices>

#include "database.h"

Database::Database(QObject *parent) : QObject(parent) {
}

/**
 * Opens a connection to thedatabase.
 *
 * @return Status of the connection. False indicates it couldn't open.
 */
bool Database::open() {
	// Build the path to the database.
#if QT_VERSION >= 0x050000
	QString db_path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
	QString db_path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
	if (!QDir(db_path).exists()) {
		QDir().mkdir(db_path);
	}

	// Set the database file.
	db_path = db_path + QDir::separator() + "database.sqlite";

	// Setup the connection.
	db = QSqlDatabase::addDatabase("QSQLITE");
	//db.setDatabaseName("/home/nathanpc/Developer/Qt/Bookmarked/bookmarked.db");
	db.setDatabaseName(db_path);

	// Open the connection to the database.
	return db.open();
}

/**
 * Creates the basic tables if the database is new.
 */
void Database::initTables() {
	QSqlQuery query;

	// Creates the Book table.
	query.exec("CREATE TABLE IF NOT EXISTS Books ("
				   "isbn varchar(18) DEFAULT NULL, "
				   "title text, "
				   "authors text, "
				   "cover blob)");

	// Creates the Bookmarks table.
	query.exec("CREATE TABLE IF NOT EXISTS Bookmarks ("
			   "isbn varchar(18) DEFAULT NULL, "
			   "page smallint(5) NOT NULL, "
			   "description text)");
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
		QString authors  = query.value(2).toString();
		QByteArray cover = query.value(3).toByteArray();

		QHash<QString, QVariant> book;
		book["isbn"]    = isbn;
		book["title"]   = title;
		book["authors"] = authors;
		book["cover"]   = cover;

		books << book;
		qDebug() << "Book:" << isbn << title << authors;
	}

	return query.size();
}

/**
 * Get the bookmarks from a book.
 *
 * @param isbn Book's ISBN.
 * @return Bookmarks list.
 */
QList<QHash<QString, QVariant> > Database::getBookmark(QString isbn) {
	QList<QHash<QString, QVariant> > bookmarks;

	// Query the database.
	QSqlQuery query;
	query.prepare("SELECT * FROM Bookmarks WHERE isbn=:isbn ORDER BY page");
	query.bindValue(":isbn", isbn);
	query.exec();

	while (query.next()) {
		QString isbn = query.value(0).toString();
		int page = query.value(1).toInt();
		QString description = query.value(2).toString();

		QHash<QString, QVariant> bookmark;
		bookmark["isbn"] = isbn;
		bookmark["page"] = page;
		bookmark["description"] = description;

		bookmarks << bookmark;
		qDebug() << "Bookmark:" << isbn << page << description;
	}

	return bookmarks;
}

/**
 * Deletes a book.
 *
 * @param isbn Book's ISBN.
 */
void Database::deleteBook(QString isbn) {
	qDebug() << "Delete Book: " << isbn;

	// Query the database.
	QSqlQuery query;
	query.prepare("DELETE FROM Books WHERE isbn=:isbn");
	query.bindValue(":isbn", isbn);
	query.exec();

	// TODO: Delete every bookmark too (ask in a dialog).
}
