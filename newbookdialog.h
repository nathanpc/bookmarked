#ifndef NEWBOOKDIALOG_H
#define NEWBOOKDIALOG_H

#include <QDialog>
#include <QNetworkReply>
#include <QHash>

#include "database.h"

namespace Ui {
	class NewBookDialog;
}

class NewBookDialog : public QDialog {
	Q_OBJECT

	private:
		Ui::NewBookDialog *ui;
		QNetworkReply *book_cover_reply;
		QNetworkReply *book_info_reply;
		QString isbndb_key;

		bool got_cover;
		bool got_info;
		QByteArray cover;
		QHash<QString, QString> info;

		void grabBookCover(QString isbn);
		void grabBookInformation(QString isbn);
		void everythingDone();

	public:
		Database *db;

		explicit NewBookDialog(QWidget *parent = 0);
		~NewBookDialog();

		void setISBNDBKey(QString key);

	private slots:
		void on_book_cover_Finished();
		void on_book_info_Finished();

	public slots:
		void accept();
};

#endif // NEWBOOKDIALOG_H
