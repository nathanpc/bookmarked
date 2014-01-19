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
	db->getBookmarks();

	// Populate the UI elements.
	populateBooks();

	//grab_book_cover("087259923X", 0);
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
		QListWidgetItem *item = new QListWidgetItem(db->books[i]["title"], listview);
		//item.setData(QPixmap::fromImage(image), Qt::DecorationRole);
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
	dialog->setISBNDBKey("KPAQI8LK");
	dialog->exec();
}
