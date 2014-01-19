#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newbookdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
										  ui(new Ui::MainWindow) {
	ui->setupUi(this);

	// Open a connection with the database.
	db = new Database();
	if (!db->open()) {
		QMessageBox::critical(this, "Database Error", "Couldn't open the database.");
	}

	// Get the books and bookmarks.
	db->getBooks();
	//db->getBookmarks();

	// Populate the UI elements.
	populateBooks();
}

MainWindow::~MainWindow() {
	delete ui;
}

/**
 * Populates the books ComboBox.
 */
void MainWindow::populateBooks() {
	QListWidget *listview = ui->books;

	for (int i = 0; i < db->books.size(); ++i) {
		// Create thumbnail.
		QImage thumbnail = QImage::fromData(db->books[i]["cover"].toByteArray());
		thumbnail = thumbnail.scaledToHeight(75);

		// Create the list item.
		QListWidgetItem *item = new QListWidgetItem(db->books[i]["title"].toString(), listview);
		item->setData(Qt::DecorationRole, QPixmap::fromImage(thumbnail));
	}
}

/**
 * Books ComboBox index has changed.
 *
 * @param index Selected index.
 */
void MainWindow::on_books_currentIndexChanged(int index) {
	if (index > 0) {
		qDebug() << "Filter by book:" << db->books[index - 1]["title"];
	} else {
		qDebug() << "All books";
	}
}

void MainWindow::on_actionNew_Book_triggered() {
	NewBookDialog *dialog = new NewBookDialog(this);
	dialog->db = db;
	dialog->setISBNDBKey("KPAQI8LK");
	dialog->exec();
}

void MainWindow::on_books_clicked(const QModelIndex &index) {
	qDebug() << "Book selected:" << index.row() << db->books[index.row()]["title"].toString();

	db->getBookmark(db->books[index.row()]["isbn"].toString());
}
