#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QJsonObject>
#include <QtNetwork>
#include <QByteArray>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QScrollBar>
#include <QPrinter>
#include <QtPrintSupport>
#include <QFileDialog>
#include <catalogitem.h>
#include <messagewidget.h>
#include <noteswidget.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(int userId,QString userName,QString preUrl,QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    ~Widget();

private:
    Ui::Widget *ui;
    bool isMove;//鼠标是否拖拽
    QPoint position;//鼠标的当前位置
    QNetworkAccessManager* httpManager1;
    QNetworkAccessManager* httpManager2;
    QNetworkAccessManager* httpManager3;
    QNetworkAccessManager* httpManager4;
    QNetworkAccessManager* httpManager5;
    QNetworkAccessManager* httpManager6;
    QNetworkAccessManager* httpManager7;
    QNetworkAccessManager* httpManager8;
    QString preUrl;
    int userId;
    QString userName;
    int catalogId;// 当前目录id
    int noteId;// 当前笔记id
    int cpower;
    // 个人文档
    QVector<QTreeWidgetItem*> items1;
    QVector<CatalogItem*> items2;
    // 共享文档
    QVector<QTreeWidgetItem*> items3;
    QVector<CatalogItem*> items4;
    NotesWidget* noteWidget;
private:// 工具函数
    void min();
    void max();
    void userInfoInit();// 用户信息初始化
    void findCatalog();// 查目录
    void deleteCatalog(int id);// 删目录
    void help(QVector<QTreeWidgetItem*>& temp1,QVector<CatalogItem*>& temp2,int id);// 辅助函数
    void newCatalog(int id,int childId);// 增目录
    void logout();// 退出登录
    void saveInfo();// 保存信息
    void deleteUser();// 注销账号
    void findNotes(int id,QString name,int power);// 根据目录查笔记
    void newNote();// 新增笔记
    void findNote(int id,QString name,int power);// 查找笔记
    void saveNote();// 保存笔记
    void download();
    void findShareNote();
};
#endif // WIDGET_H
