#include "src/include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Database db_, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db(db_)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

