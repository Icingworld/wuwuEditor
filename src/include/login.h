#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QDir>
#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include "database.h"
#include "mainwindow.h"

namespace Ui {
class login;
}

class NovelBlock : public QFrame
{
    Q_OBJECT

private:
    QString name_{}, count_{}, state_{}, cover_{};

public:
    NovelBlock(const QString & name, const QString & count, const QString & state, const QString & cover, QWidget *parent = nullptr) : QFrame(parent)
    {
        name_ = name;
        count_ = count;
        state_ = state;
        cover_ = cover;

        QVBoxLayout * layout = new QVBoxLayout(this);

        // add widgets to layout
        QLabel * nameLabel = new QLabel("书名: " + name, this);
        QLabel * countLabel = new QLabel("字数: " + count, this);
        QLabel * stateLabel = new QLabel("状态: " + state, this);
        layout->addWidget(nameLabel);
        layout->addWidget(countLabel);
        layout->addWidget(stateLabel);

        setLayout(layout);
    }
    ~NovelBlock() {};
    void mousePressEvent(QMouseEvent *event) override {
        emit sendSignal(name_, count_, state_, cover_);
        QFrame::mousePressEvent(event);
    }

signals:
    void sendSignal(const QString & name, const QString & count, const QString & state, const QString & cover);
};


class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void select(const QString & name, const QString & count, const QString & state, const QString & cover);
    void launchMainWindow(const QString & name);
    Database db{};
    QString selectName{};

private:
    Ui::login *ui;

private slots:
    void getSignal(const QString & name, const QString & count, const QString & state, const QString & cover);
    void toAdd();
    void on_createBookButton_clicked();
    void on_continueWritingButton_clicked();
    void on_exitButton_clicked();
};

#endif // LOGIN_H
