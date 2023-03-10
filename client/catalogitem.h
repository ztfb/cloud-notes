#ifndef CATALOGITEM_H
#define CATALOGITEM_H

#include <QWidget>
#include <QDebug>
#include <QJsonObject>
#include <QtNetwork>
#include <QByteArray>
#include <messagewidget.h>
#include <newsharenote.h>
namespace Ui {
class CatalogItem;
}

class CatalogItem : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogItem(int id,QString text,int userId,int fatherId,bool root,bool flag,QString preUrl,int power=-1,QWidget *parent = nullptr);
    bool eventFilter(QObject *,QEvent *);
    int getId(){return id;}
    int getFatherId(){return fatherId;}
    QString getName(){return text;}
    ~CatalogItem();

private:
    Ui::CatalogItem *ui;
    QString text;// 文字
    int id;// 主键
    int userId;
    int fatherId;
    bool root;// 是否为根
    bool flag;// 是否共享
    int power;
    QNetworkAccessManager* httpManager1;
    QNetworkAccessManager* httpManager2;
    QNetworkAccessManager* httpManager3;
    QNetworkAccessManager* httpManager4;
    QString preUrl;
private:// 工具函数
    void modifyName();
    void deleteItem();
    void createItem();
    void createNote();
    void deleteNote();
signals:
    void deleted(int id);
    void created(int id,int childId);
    void selected(int id,QString name,int power);
    void nameChanged(QString name);
    void shareCreated(int id,int nid,QString name,int power);
    void deletedNote();
};

#endif // CATALOGITEM_H
