#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>

#include "database.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	private:
		Ui::MainWindow *ui;
		Database *db;

		QNetworkReply *book_cover_reply;

		void populateBooks();
		void grabBookCover(QString isbn, int row);

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_books_currentIndexChanged(int index);
		void on_book_cover_Finished(int row);
};

#endif // MAINWINDOW_H
