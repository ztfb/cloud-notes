#ifndef NOTESWIDGET_H
#define NOTESWIDGET_H

#include <QWidget>
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QtNetwork>
#include <QByteArray>
#include <noteitem.h>
#include <messagewidget.h>
class NotesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotesWidget(QString preUrl,QWidget *parent = nullptr);
    void init(int num);// 初始化窗口
    void create(int id,QString name,QString author,QString label,QString shareCode,int defaultPower);
    void remove(int id);// 动态移除id为id的窗口
    void update(QList<int>ids,QList<QString> names,QList<QString> authors,QList<QString> labels,QList<QString> shareCodes,QList<int> defaultPowers,bool flag);
    void update(int index,QString text);
    void resizeEvent(QResizeEvent *event);
    ~NotesWidget();
private:
    QList<NoteItem*> noteItems;// 笔记项列表
    QList<int>ids;
    QList<QString> names;
    QList<QString> authors;
    QList<QString> labels;
    QList<QString> shareCodes;
    QList<int> defaultPowers;
    QNetworkAccessManager* httpManager1;
    QNetworkAccessManager* httpManager2;
    QString preUrl;
    // 当前的宽高
    int cw;
    int ch;
    int showNum;// 展示的数量
    int cid;// 当前选中id
    QString cname;
    QString cauthor;
    QString clabel;
    void sizeAndPositionUpdate();// 大小和位置更新
    void deleteItem(int id);
    void changeItem(int id,QString name,QString author,QString label);
signals:
    void selected(int id,QString name,int power);
public slots:
};

#endif // NOTESWIDGET_H
