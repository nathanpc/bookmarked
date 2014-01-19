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
	QHash<QString, QVariant> book = db->books[index.row()];
	qDebug() << "Book selected:" << index.row() << book["title"].toString();

	// Populate the view.
	ui->title_label->setText(book["title"].toString());
	ui->authors_label->setText(book["authors"].toString());

	// Set the book cover.
	QImage image = QImage::fromData(book["cover"].toByteArray()).scaledToHeight(140);
	QPixmap pixmap = QPixmap::fromImage(image);
	ui->cover_label->setPixmap(pixmap);

	// Get the bookmarks.
	QString isbn = book["isbn"].toString();
	QSqlTableModel *model = new QSqlTableModel(this, db->db);
	model->setTable("Bookmarks");
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setFilter("isbn=\"" + isbn + "\"");
	model->setSort(1, Qt::AscendingOrder);
	model->select();

	model->setHeaderData(1, Qt::Horizontal, tr("Page"));
	model->setHeaderData(2, Qt::Horizontal, tr("Description"));

	ui->tableView->setModel(model);
	ui->tableView->hideColumn(0);
	ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_actionRemove_selected_bookmark_triggered() {
	QMessageBox::information(this, "Not yet implemented", "Sorry, this feature hasn't been implemented yet.");
}

void MainWindow::on_actionDelete_Current_Book_triggered() {
	QMessageBox::information(this, "Not yet implemented", "Sorry, this feature hasn't been implemented yet.");
}
