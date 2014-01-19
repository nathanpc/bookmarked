#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "database.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	private:
		Ui::MainWindow *ui;
		Database *db;

		void populateBooks();

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_books_currentIndexChanged(int index);
		void on_actionNew_Book_triggered();
		void on_books_clicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
