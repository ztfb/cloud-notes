#ifndef NEWSHARENOTE_H
#define NEWSHARENOTE_H

#include <QDialog>
#include <QJsonObject>
#include <QtNetwork>
#include <QByteArray>
#include <messagewidget.h>
namespace Ui {
class NewShareNote;
}

class NewShareNote : public QDialog
{
    Q_OBJECT

public:
    explicit NewShareNote(QString preUrl,QWidget *parent = nullptr);
    ~NewShareNote();

private:
    Ui::NewShareNote *ui;
    QString preUrl;
    QNetworkAccessManager* httpManager1;
signals:
    void shareCreated(int id,int nid,QString name,int power);
};

#endif // NEWSHARENOTE_H
