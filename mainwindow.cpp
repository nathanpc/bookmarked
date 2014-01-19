#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

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
	db->getBooks();
	db->getBookmarks();

	// Populate the UI elements.
	populateBooks();

	grab_book_cover("087259923X", 0);
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
 * Grabs the book cover image from the internet.
 *
 * @param isbn Book's ISBN-10.
 * @param row Item row to insert the image.
 */
void MainWindow::grabBookCover(QString isbn, int row) {
	// Setup the connection.
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QUrl url = QUrl("http://images.amazon.com/images/P/" + isbn + ".01.THUMBZZZ.jpg");
	book_cover_reply = manager->get(QNetworkRequest(url));

	// Setup the signals and slots.
	QSignalMapper* signalMapper = new QSignalMapper (this);
	connect(book_cover_reply, SIGNAL(finished()), signalMapper, SLOT(map()));
	signalMapper->setMapping(book_cover_reply, row);
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(on_book_cover_Finished(int)));
}

void MainWindow::on_book_cover_Finished(int row) {
	// Grab the image data.
	QByteArray data = book_cover_reply->readAll();
	QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data));

	// Insert the image into the list item.
	ui->books->item(row)->setData(Qt::DecorationRole, pixmap);
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
