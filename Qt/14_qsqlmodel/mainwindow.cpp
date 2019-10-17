#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlTableModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("communes.db");
    if (!db.open())
        exit(EXIT_FAILURE);
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable("communes");
    model->select();
    ui->tableView->setModel(model);
    connect(ui->tableView, &QTableView::pressed, this, &MainWindow::mDisplayValue);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mDisplayValue(const QModelIndex &idx)
{
    ui->lineEdit->setText(idx.data().toString());
}
