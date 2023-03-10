#include "noteitem.h"
#include "ui_noteitem.h"

NoteItem::NoteItem(QString preUrl,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteItem)
{
    ui->setupUi(this);
    this->preUrl=preUrl;
    this->setAttribute(Qt::WA_TranslucentBackground);// 设置窗体透明
    connect(ui->deleteButton,&QToolButton::clicked,this,[=](){
        this->deleted(this->id);
    });
    connect(ui->editButton,&QToolButton::clicked,this,[=](){
        NoteEdit noteEdit(this->id,this->name,this->author,this->label);
        connect(&noteEdit,&NoteEdit::change,this,&NoteItem::change);
        noteEdit.exec();
    });
    connect(ui->shareButton,&QToolButton::clicked,this,[=](){
        ShareWidget shareWidget(this->id,this->shareCode,this->defaultPower,this->preUrl);
        connect(&shareWidget,&ShareWidget::powerChanged,this,[=](int power){
            this->defaultPower=power;
        });
        shareWidget.exec();
    });
}
void NoteItem::reAssign(int id, QString name, QString author, QString label, QString shareCode, int defaultPower)
{
    this->id=id;
    this->name=name;
    this->author=author;
    this->label=label;
    this->shareCode=shareCode;
    this->defaultPower=defaultPower;
}
void NoteItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->selected(this->id,this->name,-1);
}
void NoteItem::update(){
    ui->label_3->setText(name);
    ui->label->setText(author+"的笔记");
    ui->label_2->setText(label);
}
NoteItem::~NoteItem()
{
    delete ui;
}
