#ifndef SHAREWIDGET_H
#define SHAREWIDGET_H

#include <QDialog>
#include <QClipboard>
#include <QJsonObject>
#include <QtNetwork>
#include <QByteArray>
#include <messagewidget.h>
namespace Ui {
class ShareWidget;
}

class ShareWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ShareWidget(int id,QString shareCode,int defaultPower,QString preUrl,QWidget *parent = nullptr);
    ~ShareWidget();

private:
    Ui::ShareWidget *ui;
    int id;
    QString shareCode;
    int defaultPower;
    QString preUrl;
    QNetworkAccessManager* httpManager1;
signals:
    void powerChanged(int power);
};

#endif // SHAREWIDGET_H
