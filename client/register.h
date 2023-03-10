#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QMouseEvent>
#include <QJsonObject>
#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include <messagewidget.h>
#include <widget.h>
namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QString preUrl,QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    ~Register();

private:
    Ui::Register *ui;
    bool isMove;//鼠标是否拖拽
    QPoint position;//鼠标的当前位置
    QNetworkAccessManager* httpManager1;
    QNetworkAccessManager* httpManager2;
    QString preUrl;
private:// 工具函数
    void min();
    void max();
    void login();
    void reg();
};

#endif // REGISTER_H
