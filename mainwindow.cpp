#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
										  ui(new Ui::MainWindow) {
	ui->setupUi(this);

	// Open a connection with the database.
	db = new Database();
	if (!db->open()) {
		QMessageBox::critical(this, "Database Error", "Couldn't open the database.");
	}

	// Get the books and bookmarks.
	db->get_books();
	db->get_bookmarks();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::populate_books() {
	//
}
