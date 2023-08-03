#include "src/include/login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    // set UI
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    
    db.setFile("data/user.db");
    QSqlQuery query = db.query();
    QDir dataDir("data");
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
        qDebug() << "Created 'data' folder.";
        if (db.open())
        {
            query.exec("CREATE TABLE user (name TEXT NOT NULL, count INTEGER NOT NULL);");
            query.exec("CREATE TABLE book (name TEXE NOT NULL, count INTEGER NOT NULL, state INTEGER NOT NULL, cover TEXT);");
            query.exec("INSERT INTO user (name, count) VALUES (\"wuwuCreator\", 0);");
        }
    }
    QDir dataDir1("data/book");
    if (!dataDir1.exists()) {
        dataDir1.mkpath(".");
    }
    if (db.open())
    {
        query.prepare("SELECT name, count FROM user;");
        if (query.exec())
        {
            while (query.next()) {
                QString name = query.value(0).toString();
                QString count = query.value(1).toString();
                ui->userName->setText("用户名: " + name);
                ui->userCount->setText("总字数: " + count);
            }
        }
        query.prepare("SELECT name, count, state, cover FROM book;");
        if (query.exec())
        {
            bool allCompleted{true};
            NovelBlock * ptr = nullptr;
            while (query.next()) {
                QString name = query.value(0).toString();
                QString count = query.value(1).toString();
                QString state = query.value(2).toString();
                QString cover = query.value(3).toString();
                if (state == "0")
                {
                    allCompleted = false;
                    state = "连载中";
                } else {
                    state = "已完结";
                }
                if (cover == "")
                {
                    cover = ":/img/img/default.png";
                }
                NovelBlock * newBlock = new NovelBlock(name, count, state, cover, this);
                ui->bookListLayout->insertWidget(0, newBlock);
                connect(newBlock, &NovelBlock::sendSignal, this, &login::getSignal);
                ptr = newBlock;
            }
            if (ui->bookListLayout->count() <= 1 || ptr == nullptr)
            {
                QLabel * note = new QLabel("还没有进行创作", this);
                note->setAlignment(Qt::AlignHCenter);
                ui->bookListLayout->insertWidget(0, note);
            } else {
                QMouseEvent * mouseEvent = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                ptr->mousePressEvent(mouseEvent);
            }
            if (allCompleted)
            {
                ui->stackedWidget->setCurrentIndex(1);
            }
            QPushButton * addBook = new QPushButton("+", this);
            ui->bookListLayout->insertWidget(0, addBook);
            connect(addBook, &QPushButton::clicked, this, &login::toAdd);
        }
    }
}

login::~login()
{
    delete ui;
    db.close();
}

void login::select(const QString & name, const QString & count, const QString & state, const QString & cover)
{
    ui->stackedWidget->setCurrentIndex(0);
    QPixmap pixmap(cover);
    ui->cover->setPixmap(pixmap);
    ui->cover->show();
    selectName = name;
    ui->bookName->setText("书名: " + name);
    ui->bookCount->setText("字数: " + count);
    ui->bookState->setText("状态: " + state);
}

void login::getSignal(const QString & name, const QString & count, const QString & state, const QString & cover)
{
    select(name, count, state, cover);
}

void login::toAdd()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void login::createDir(const QString & path)
{
    QDir dataDir(path);
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }
}

void login::on_createBookButton_clicked()
{
    QString newName = ui->newBookName->text();
    if (newName == "")
    {
        return;
    }
    QSqlQuery query = db.query();
    query.prepare("INSERT INTO book (name, count, state) VALUES (:name, 0, 0);");
    query.bindValue(":name", newName);
    query.exec();
    createDir("data/book/" + newName);
    createDir("data/book/" + newName + "/chapter");
    createDir("data/book/" + newName + "/note");
    createDir("data/book/" + newName + "/chapterNote");
    createDir("data/book/" + newName + "/output");
    createDir("data/book/" + newName + "/output/txt");
    createDir("data/book/" + newName + "/output/markdown");
    launchMainWindow(newName);
}

void login::on_continueWritingButton_clicked()
{
    launchMainWindow(selectName);
}

void login::on_exitButton_clicked()
{
    QWidget::close();
}

void login::launchMainWindow(const QString & name)
{
    Database dbBook;
    dbBook.setFile("data/book/" + name + "/book.db");
    if (dbBook.open())
    {
        QSqlQuery query = dbBook.query();
        query.exec("CREATE TABLE IF NOT EXISTS content (chapter TEXT NOT NULL, count INTEGER NOT NULL, state INTEGER NOT NULL);");
        // launch MainWindow
        MainWindow * m = new MainWindow(dbBook);
        m->show();
        QWidget::close();
    } else {
        return;
    }
}
