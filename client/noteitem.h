#ifndef NOTEITEM_H
#define NOTEITEM_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <noteedit.h>
#include <sharewidget.h>
namespace Ui {
class NoteItem;
}

class NoteItem : public QWidget
{
    Q_OBJECT

public:
    explicit NoteItem(QString preUrl,QWidget *parent = nullptr);
    void reAssign(int id,QString name,QString author,QString label,QString shareCode,int defaultPower);
    void update();// 更新
    void mouseDoubleClickEvent(QMouseEvent *event);
    ~NoteItem();

private:
    Ui::NoteItem *ui;
    int id;
    QString name;
    QString author;
    QString label;
    QString shareCode;
    int defaultPower;
    QString preUrl;
signals:
    void selected(int id,QString name,int power);
    void deleted(int id);
    void change(int id,QString name,QString author,QString label);
};

#endif // NOTEITEM_H
