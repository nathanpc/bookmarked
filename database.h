#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QList>
#include <QHash>

class Database : public QObject {
	Q_OBJECT

	private:
		QSqlDatabase db;

	public:
		QString error_msg;
		QList<QHash<QString, QVariant> > books;

		explicit Database(QObject *parent = 0);

		bool open();
		int getBooks();
		QList<QHash<QString, QVariant> > getBookmark(QString isbn);

	signals:

	public slots:

};

#endif // DATABASE_H
