#include "noteedit.h"
#include "ui_noteedit.h"

NoteEdit::NoteEdit(int id,QString name,QString author,QString label,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoteEdit)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);// 去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);// 设置窗体透明
    ui->lineEdit->setText(name);
    ui->lineEdit_2->setText(author);
    ui->lineEdit_3->setText(label);
    connect(ui->toolButton_2,&QToolButton::clicked,this,&NoteEdit::close);
    connect(ui->toolButton,&QToolButton::clicked,this,[=](){
        if(ui->lineEdit->text()==""){
            MessageWidget message;
            message.setText("未输入名称","确定","取消");
            message.exec();
        }else if(ui->lineEdit_2->text()==""){
            MessageWidget message;
            message.setText("未输入作者","确定","取消");
            message.exec();
        }else if(ui->lineEdit_3->text()==""){
            MessageWidget message;
            message.setText("未输入标签","确定","取消");
            message.exec();
        }else this->change(id,ui->lineEdit->text(),ui->lineEdit_2->text(),ui->lineEdit_3->text());
    });
}

NoteEdit::~NoteEdit()
{
    delete ui;
}
