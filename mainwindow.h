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
		void on_actionRemove_selected_bookmark_triggered();
		void on_actionDelete_Current_Book_triggered();
};

#endif // MAINWINDOW_H
