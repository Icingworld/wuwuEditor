#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTreeView>
#include <QModelIndex>
#include <QStandardItem>
#include <QDate>
#include <QTime>
#include <QMenu>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QMessageBox>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Database db_, QWidget *parent = nullptr);
    ~MainWindow();
    void addChapter(const QString & chapterName);
    void getChapterList();
    void countAll();
    QString getTime();
    void createFile(const QString & path);
    void readFile(const QString & path, QTextEdit * content);
    void saveToFile(const QString & path, QTextEdit * content);
    void Warning(const QString & string);
    int chooseBox(const QString & string);

private slots:
    void on_toEditButton_clicked();
    void on_toNoteButton_clicked();
    void on_searchButton_clicked();
    void on_addChapterButton_clicked();
    void on_catalogueView_clicked(const QModelIndex &index);
    void on_saveButton_clicked();
    void on_completeButton_clicked();
    void on_titleButton_clicked();
    void on_content_textChanged();
    void on_chapterNote_textChanged();

private:
    Ui::MainWindow *ui;
    Database db;
    QString path{};
    QString rawName{};
    bool isAllCompleted{true};
    bool isSaved{true};
    QStandardItemModel * model;
    QStandardItem * rootItem;
};
#endif // MAINWINDOW_H
