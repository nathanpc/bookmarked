#ifndef ADDBOOKMARKDIALOG_H
#define ADDBOOKMARKDIALOG_H

#include <QDialog>

#include "database.h"

namespace Ui {
	class AddBookmarkDialog;
}

class AddBookmarkDialog : public QDialog {
	Q_OBJECT

	private:
		Ui::AddBookmarkDialog *ui;

	public:
		Database *db;
		QString isbn;

		explicit AddBookmarkDialog(QWidget *parent = 0);
		~AddBookmarkDialog();

	public slots:
		void accept();
};

#endif // ADDBOOKMARKDIALOG_H
