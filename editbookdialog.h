#ifndef EDITBOOKDIALOG_H
#define EDITBOOKDIALOG_H

#include <QDialog>

namespace Ui {
	class EditBookDialog;
}

class EditBookDialog : public QDialog {
	Q_OBJECT

	private:
		Ui::EditBookDialog *ui;
		QString isbn;

	public:
		explicit EditBookDialog(QWidget *parent = 0);
		~EditBookDialog();

		void populateEdits(QString title, QString authors);
		void setISBN(QString isbn);
	private slots:
		void on_buttonBox_accepted();
};

#endif // EDITBOOKDIALOG_H
