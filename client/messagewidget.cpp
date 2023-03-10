#include "messagewidget.h"
#include "ui_messagewidget.h"

MessageWidget::MessageWidget(bool* select,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);// 去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);// 设置窗体透明
    connect(ui->toolButton,&QToolButton::clicked,this,[=](){
        if(select!=nullptr)*select=true;
        this->close();
    });
    connect(ui->toolButton_2,&QToolButton::clicked,this,[=](){
        if(select!=nullptr)*select=false;
        this->close();
    });
}
void MessageWidget::setText(QString text1,QString text2,QString text3)
{
    ui->label->setText(text1);
    ui->toolButton->setText(text2);
    ui->toolButton_2->setText(text3);
}
MessageWidget::~MessageWidget()
{
    delete ui;
}
