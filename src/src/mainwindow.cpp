#include "src/include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Database db_, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db(db_)
{
    ui->setupUi(this);
    ui->title->setDisabled(true);
    ui->content->setDisabled(true);
    ui->chapterNote->setDisabled(true);
    ui->title->setText("标题(未选择章节)");
    ui->content->setText("正文(未选择章节)");
    ui->chapterNote->setText("备注(未选择章节)");
    ui->countLabel->setText("字数：-   ");
    ui->lastSaveTimeLabel->setText("最后保存时间：-");
    isSaved = true;

    // set path
    path = db.name().chopped(7);
    getChapterList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* this is global part */

void MainWindow::countAll()
{
    // update user.db
}

QString MainWindow::getTime()
{
    QString timeNow{};
    QDate currentDate = QDate::currentDate();
    timeNow += currentDate.toString("yyyy-MM-dd ");
    QTime currentTime = QTime::currentTime();
    timeNow += currentTime.toString("hh:mm:ss");
    return timeNow;
}

void MainWindow::createFile(const QString & path)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    { 
    }
    file.close();
}

void MainWindow::readFile(const QString & path, QTextEdit * content)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            content->insertPlainText(line);
        }
        file.close();
    }
    else
    {
        qDebug() << "Failed to open the file.";
    }
}

void MainWindow::saveToFile(const QString & path, QTextEdit * content)
{
    QString filePath = path;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << content->toPlainText();
        file.close();
    }
}

void MainWindow::Warning(const QString & string)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("提示");
    messageBox.setText(string);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.addButton("确认", QMessageBox::AcceptRole);
    messageBox.exec();
}

int MainWindow::chooseBox(const QString & string)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("提示");
    messageBox.setText(string);
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Ok);
    int ret = messageBox.exec();
    if (ret == QMessageBox::Ok) {
        return 1;
    } else if (ret == QMessageBox::Cancel) {
        return 0;
    }
    return 1;
}

void MainWindow::on_toEditButton_clicked()
{
    ui->leftStacked->setCurrentIndex(0);
    ui->rightStacked->setCurrentIndex(0);
}

void MainWindow::on_toNoteButton_clicked()
{
    ui->leftStacked->setCurrentIndex(1);
    ui->rightStacked->setCurrentIndex(1);
}

/* this part belong to edit */

void MainWindow::addChapter(const QString & chapterName)
{
    QStandardItem * item = new QStandardItem(chapterName); // create a new node
    rootItem->appendRow(item);
}

void MainWindow::getChapterList()
{
    model = new QStandardItemModel(this);
    ui->catalogueView->setModel(model);
    rootItem = model->invisibleRootItem();
    QSqlQuery query = db.query();
    query.prepare("SELECT chapter, count, state FROM content;");
    if (query.exec())
    {
        while (query.next()) {
            QString chapterName = query.value(0).toString();
            int state = query.value(2).toInt();
            if (state == 0)
            {
                // not completed
                isAllCompleted = false;
                chapterName += "(未完成)";
            }
            addChapter(chapterName);
        }
    }
}

void MainWindow::on_searchButton_clicked()
{
    
}

void MainWindow::on_addChapterButton_clicked()
{
    if (isAllCompleted)
    {
        isAllCompleted = false;
        // create a new node
        QStandardItem * item = new QStandardItem("新章节(未完成)");
        rootItem->appendRow(item);
        // create new file
        createFile(path + "chapter/新章节.txt");
        createFile(path + "chapterNote/新章节.txt");
        // update database
        db.open();
        QSqlQuery query = db.query();
        query.prepare("INSERT INTO content (chapter, count, state) VALUES (:chapterName, 0, 0);");
        query.bindValue(":chapterName", "新章节");
        query.exec();
    } else {
        Warning("存在未完成章节！");
    }
}

void MainWindow::on_catalogueView_clicked(const QModelIndex & index)
{
    if (!isSaved)
    {
        int ret = chooseBox("未保存，是否保存？");
        if (ret == 1)
        {
            on_saveButton_clicked();
        }
    }
    ui->content->setEnabled(true);
    ui->content->clear();
    ui->title->setDisabled(true);
    ui->title->clear();
    ui->chapterNote->setEnabled(true);
    ui->chapterNote->clear();
    QStandardItem * item = model->itemFromIndex(index);
    if (item != nullptr)
    {
        QString title = item->text().replace("(未完成)", "");
        ui->title->setText(title);
        readFile(path + "chapter/" + title + ".txt", ui->content);
        readFile(path + "chapterNote/" + title + ".txt", ui->chapterNote);
    }
    ui->lastSaveTimeLabel->setText("最后保存时间：" + getTime());
    isSaved = true;
}

void MainWindow::on_saveButton_clicked()
{
    isSaved = true;
    // save to file
    saveToFile(path + "chapter/" + ui->title->text() + ".txt", ui->content);
    saveToFile(path + "chapterNote/" + ui->title->text() + ".txt", ui->chapterNote);
    // update count
    db.open();
    QSqlQuery query = db.query();
    query.prepare("UPDATE content SET count = :count WHERE chapter = :title;");
    query.bindValue(":count", ui->content->toPlainText().count());
    query.bindValue(":title", ui->title->text());
    query.exec();
    ui->lastSaveTimeLabel->setText("最后保存时间：" + getTime());
}

void MainWindow::on_completeButton_clicked()
{
    db.open();
    QSqlQuery query = db.query();
    query.prepare("UPDATE content SET state = 1 WHERE chapter = :title;");
    query.bindValue(":title", ui->title->text());
    if (query.exec())
    {
        getChapterList();
    }
}

void MainWindow::on_titleButton_clicked()
{
    if (ui->title->isEnabled())
    {
        if (ui->title->text() == "")
        {
            ui->title->setText(rawName);
            return;
        }
        ui->title->setDisabled(true);
        ui->titleButton->setText("修改");
        db.open();
        QSqlQuery query = db.query();
        query.prepare("UPDATE content SET chapter = :newName WHERE chapter = :rawName;");
        query.bindValue(":newName", ui->title->text());
        query.bindValue(":rawName", rawName);
        if (query.exec())
        {
            // rename file
            QString oldFilePath = path + "chapter/" + rawName + ".txt";
            QString newFilePath = path + "chapter/" + ui->title->text() + ".txt";
            QFile file(oldFilePath);
            if (file.rename(newFilePath)) {
                qDebug() << "File renamed successfully";
            } else {
                qDebug() << "Failed to rename file";
            }
            // refresh list
            getChapterList();
        }
    } else {
        ui->title->setEnabled(true);
        ui->titleButton->setText("确认");
        rawName = ui->title->text();
    }
}

void MainWindow::on_content_textChanged()
{
    isSaved = false;
    QString text = ui->content->toPlainText();
    int count = text.count();
    ui->countLabel->setText("字数：" + QString::number(count) + "   ");
}

void MainWindow::on_chapterNote_textChanged()
{
    isSaved = false;
}

/* this part belong to note */