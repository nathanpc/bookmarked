#include <QMessageBox>
#include <QDesktopServices>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newbookdialog.h"
#include "addbookmarkdialog.h"
#include "aboutdialog.h"
#include "editbookdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
										  ui(new Ui::MainWindow) {
	ui->setupUi(this);
	ui->detail_view->hide();
	setupCustomMenus();

	// Open a connection with the database.
	db = new Database();
	if (!db->open()) {
		QMessageBox::critical(this, tr("Database Error"), tr("Couldn't open the database."));
	}

	// Get the books and populate the list.
	populateBooks();
}

MainWindow::~MainWindow() {
	delete ui;
}

/**
 * Setup custom menus.
 */
void MainWindow::setupCustomMenus() {
	// Delete action for the books.
	QAction *action_delbook = new QAction(ui->books);
	action_delbook->setText("Delete");
	action_delbook->setShortcut(QKeySequence::Delete);
	connect(action_delbook, SIGNAL(triggered()), this, SLOT(on_actionDelete_Current_Book_triggered()));
	ui->books->addAction(action_delbook);

	// Delete action for the bookmarks.
	QAction *action_delbookmark = new QAction(ui->tableView);
	action_delbookmark->setText("Delete");
	action_delbookmark->setShortcut(QKeySequence::Delete);
	connect(action_delbookmark, SIGNAL(triggered()), this, SLOT(on_actionRemove_selected_bookmark_triggered()));
	ui->tableView->addAction(action_delbookmark);
}

/**
 * Populates the books ComboBox.
 */
void MainWindow::populateBooks() {
	QListWidget *listview = ui->books;
	listview->clear();

	// Fetch the books.
	db->getBooks();

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
 * Populate the bookmarks into the table view.
 *
 * @param isbn The ISBN of the book.
 */
void MainWindow::populateBookmarks(QString isbn) {
	// Get the bookmarks.
	QSqlTableModel *model = new QSqlTableModel(this, db->db);
	model->setTable("Bookmarks");
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setFilter("isbn=\"" + isbn + "\"");
	model->setSort(1, Qt::AscendingOrder);
	model->select();

	// Rename the headers.
	model->setHeaderData(1, Qt::Horizontal, tr("Page"));
	model->setHeaderData(2, Qt::Horizontal, tr("Description"));

	// Set the model and adjust the table.
	ui->tableView->setModel(model);
	ui->tableView->hideColumn(0);
	ui->tableView->resizeColumnsToContents();
}

/**
 * Populate the detail view.
 *
 * @param book Books thingy.
 */
void MainWindow::populateDetailView(QHash<QString, QVariant> book) {
	// Populate the view.
	ui->title_label->setText(book["title"].toString());
	ui->authors_label->setText(book["authors"].toString());

	// Set the book cover.
	QImage image = QImage::fromData(book["cover"].toByteArray()).scaledToHeight(140);
	QPixmap pixmap = QPixmap::fromImage(image);
	ui->cover_label->setPixmap(pixmap);

	// Populate the bookmarks table.
	populateBookmarks(book["isbn"].toString());

	ui->detail_view->show();
}

/**
 * Books list index has changed.
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

/**
 * Launches the new book dialog.
 */
void MainWindow::on_actionNew_Book_triggered() {
	NewBookDialog *dialog = new NewBookDialog(this);
	dialog->db = db;
	dialog->setISBNDBKey("KPAQI8LK");
	dialog->show();

	connect(dialog, SIGNAL(finished(int)), this, SLOT(on_NewBookDialog_accepted(int)));
}

/**
 * A book was selected by the user.
 *
 * @param index Index of the selected item.
 */
void MainWindow::on_books_clicked(const QModelIndex &index) {
	QHash<QString, QVariant> book = db->books[index.row()];
	qDebug() << "Book selected:" << index.row() << book["title"].toString();

	populateDetailView(book);
}

/**
 * The user wants to delete a bookmark.
 */
void MainWindow::on_actionRemove_selected_bookmark_triggered() {
	int row = ui->tableView->selectionModel()->currentIndex().row();
	qDebug() << "Deleting bookmark row: " << row;

	if (row >= 0) {
		ui->tableView->model()->removeRow(row);

		// Refresh the table.
		QModelIndexList indexes = ui->books->selectionModel()->selectedIndexes();
		populateBookmarks(db->books[indexes[0].row()]["isbn"].toString());
	} else {
		QMessageBox::information(this, tr("Bad User"), tr("No bookmark selected"));
	}
}

/**
 * The user wants to delete a book.
 */
void MainWindow::on_actionDelete_Current_Book_triggered() {
	QModelIndexList indexes = ui->books->selectionModel()->selectedIndexes();

	if (indexes.length() > 0) {
		int row = indexes[0].row();
		db->deleteBook(db->books[row]["isbn"].toString());
	} else {
		QMessageBox::information(this, tr("Bad User"), tr("No book selected"));
	}

	populateBooks();
	ui->detail_view->hide();
}

/**
 * Donate via PayPal.
 */
void MainWindow::on_actionPayPal_triggered() {
	QUrl url = QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business="
					"eeepc904%40gmail%2ecom&lc=BR&item_name=Bookmarked%20Developer"
					"%20%28Nathan%20Campos%29&item_number=BookmarkedDonation&"
					"currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted");
	QDesktopServices::openUrl(url);
}

/**
 * Donate via Flattr.
 */
void MainWindow::on_actionFlattr_triggered() {
	QUrl url = QUrl("https://flattr.com/profile/nathanpc");
	QDesktopServices::openUrl(url);
}

/**
 * Opens the add bookmark dialog.
 */
void MainWindow::on_add_bookmark_clicked() {
	QModelIndexList indexes = ui->books->selectionModel()->selectedIndexes();

	if (indexes.length() > 0) {
		AddBookmarkDialog *dialog = new AddBookmarkDialog();
		dialog->db = db;
		dialog->isbn = db->books[indexes[0].row()]["isbn"].toString();
		dialog->show();

		connect(dialog, SIGNAL(finished(int)), this, SLOT(on_AddBookmarkDialog_accepted(int)));
	}
}

/**
 * The user added a new bookmark and we should refresh the model.
 *
 * @param status Return status.
 */
void MainWindow::on_AddBookmarkDialog_accepted(int status) {
	if (status == QDialog::Accepted) {
		QModelIndexList indexes = ui->books->selectionModel()->selectedIndexes();
		populateBookmarks(db->books[indexes[0].row()]["isbn"].toString());
	}
}

/**
 * The user probably added a new book, refresh the books.
 *
 * @param status Return status.
 */
void MainWindow::on_NewBookDialog_accepted(int status) {
	if (status == QDialog::Accepted) {
		populateBooks();
	}
}

/**
 * The user probably edited a book, refresh the books.
 *
 * @param status Return status.
 */
void MainWindow::on_EditBookDialog_accepted(int status) {
	if (status == QDialog::Accepted) {
		populateBooks();
		ui->detail_view->hide();
	}
}

/**
 * Show the about dialog.
 */
void MainWindow::on_actionAbout_triggered() {
	AboutDialog *dialog = new AboutDialog();
	dialog->show();
}

/**
 * Contact the developer.
 */
void MainWindow::on_actionContact_Developer_triggered() {
	QUrl url = QUrl("mailto:nathanpc@dreamintech.net");
	QDesktopServices::openUrl(url);
}

/**
 * Edit the book details.
 */
void MainWindow::on_edit_details_clicked() {
	QModelIndexList indexes = ui->books->selectionModel()->selectedIndexes();
	QString isbn = db->books[indexes[0].row()]["isbn"].toString();

	EditBookDialog *dialog = new EditBookDialog();
	dialog->setISBN(isbn);
	dialog->populateEdits(ui->title_label->text(), ui->authors_label->text());
	dialog->show();

	connect(dialog, SIGNAL(finished(int)), this, SLOT(on_EditBookDialog_accepted(int)));
}
