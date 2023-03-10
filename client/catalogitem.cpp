#include "catalogitem.h"
#include "ui_catalogitem.h"

CatalogItem::CatalogItem(int id,QString text,int userId,int fatherId,bool root,bool flag,QString preUrl,int power,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogItem)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    this->id=id;this->text=text;this->userId=userId;this->fatherId=fatherId;this->root=root;this->preUrl=preUrl;this->flag=flag;this->power=power;
    ui->lineEdit->setText(text);
    if(flag==false)ui->lineEdit->setReadOnly(true);
    httpManager1 = new QNetworkAccessManager(this);
    httpManager2 = new QNetworkAccessManager(this);
    httpManager3 = new QNetworkAccessManager(this);
    httpManager4 = new QNetworkAccessManager(this);
    if(this->flag)connect(ui->lineEdit,&QLineEdit::editingFinished,this,&CatalogItem::modifyName);
    if(this->flag||this->root)connect(ui->toolButton,&QToolButton::clicked,this,&CatalogItem::deleteItem);
    else connect(ui->toolButton,&QToolButton::clicked,this,&CatalogItem::deleteNote);
    if(this->flag)connect(ui->toolButton_2,&QToolButton::clicked,this,&CatalogItem::createItem);
    else if(!this->flag&&this->root) connect(ui->toolButton_2,&QToolButton::clicked,this,&CatalogItem::createNote);// 只有根可以响应
}
bool CatalogItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==ui->lineEdit){
        if(event->type()==QEvent::MouseButtonDblClick&&flag)ui->lineEdit->setReadOnly(false);
        else if(event->type()==QEvent::FocusOut)ui->lineEdit->setReadOnly(true);
        else if(event->type()==QEvent::MouseButtonPress){
            if(this->flag)this->selected(this->id,this->text,this->power);
            else this->selected(this->userId,this->text,this->power);
        }
    }
    return QWidget::eventFilter(watched,event);
}
void CatalogItem::modifyName()
{
    if(text!=ui->lineEdit->text()){
        QNetworkRequest httpRequest;
        QString fullUrl=preUrl+"/catalog/update";
        httpRequest.setUrl(QUrl(fullUrl));
        httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        QJsonObject tempObj;
        tempObj.insert("id",QString::number(id));
        tempObj.insert("name",ui->lineEdit->text());
        tempObj.insert("userId",QString::number(userId));
        tempObj.insert("fatherId",QString::number(fatherId));
        QJsonDocument tempDoc;
        tempDoc.setObject(tempObj);
        QByteArray data = tempDoc.toJson(QJsonDocument::Compact);
        httpManager1->post(httpRequest,data);
        connect(httpManager1,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
            if(httpReply->error()!=QNetworkReply::NoError){
                MessageWidget message;
                message.setText("网络好像断开了","确定","取消");
                message.exec();
            }else{
                this->text=ui->lineEdit->text();
                this->nameChanged(this->text);
            }
        });
    }
}
void CatalogItem::deleteItem()
{
    if(root){
        MessageWidget message;
        message.setText("无法删除根目录","确定","取消");
        message.exec();
    }else{
        // 删除选中的项
        bool select;
        MessageWidget message(&select);
        message.setText("确定删除？","确定","取消");
        message.exec();
        if(select){
            QNetworkRequest httpRequest;
            QString fullUrl=preUrl+"/catalog/delete?"+"id="+QString::number(id);
            httpRequest.setUrl(QUrl(fullUrl));
            httpManager2->get(httpRequest);
            connect(httpManager2,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
                if(httpReply->error()!=QNetworkReply::NoError){
                    MessageWidget message;
                    message.setText("网络好像断开了","确定","取消");
                    message.exec();
                }else{
                    //获取响应信息
                    const QByteArray data=httpReply->readAll();
                    // 解析响应结果
                    QJsonParseError jsonError;
                    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));
                    if(jsonError.error != QJsonParseError::NoError)return;
                    QJsonObject rootObj = jsonDoc.object();
                    if(rootObj.value("code").toString()=="0"){
                        MessageWidget message;
                        message.setText(rootObj.value("msg").toString(),"确定","取消");
                        message.exec();
                    }
                    this->deleted(this->id);
                }
            });
        }
    }
}
void CatalogItem::createItem()
{
    QNetworkRequest httpRequest;
    QString fullUrl=preUrl+"/catalog/create";
    httpRequest.setUrl(QUrl(fullUrl));
    httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QJsonObject tempObj;
    tempObj.insert("name","新建文件夹");
    tempObj.insert("userId",userId);
    tempObj.insert("fatherId",id);
    QJsonDocument tempDoc;
    tempDoc.setObject(tempObj);
    QByteArray data = tempDoc.toJson(QJsonDocument::Compact);
    httpManager3->post(httpRequest,data);
    connect(httpManager3,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
        if(httpReply->error()!=QNetworkReply::NoError){
            MessageWidget message;
            message.setText("网络好像断开了","确定","取消");
            message.exec();
        }else{
            //获取响应信息
            const QByteArray data=httpReply->readAll();
            // 解析响应结果
            QJsonParseError jsonError;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));
            if(jsonError.error != QJsonParseError::NoError)return ;
            QJsonObject rootObj = jsonDoc.object();
            if(rootObj.value("code").toString()=="0"){
                int childId=rootObj.value("data").toInt();
                this->created(id,childId);
            }
        }
    });
}
void CatalogItem::createNote(){
    // 新增共享笔记
    NewShareNote newShareNote(preUrl);
    connect(&newShareNote,&NewShareNote::shareCreated,this,&CatalogItem::shareCreated);
    newShareNote.exec();
}
void CatalogItem::deleteNote(){
    // 这里用userId表示nid,用id表示关系的主键id
    // 删除选中的项
    bool select;
    MessageWidget message(&select);
    message.setText("确定删除？","确定","取消");
    message.exec();
    if(select){
        QNetworkRequest httpRequest;
        QString fullUrl=preUrl+"/shareNote/delete?"+"id="+QString::number(id);
        httpRequest.setUrl(QUrl(fullUrl));
        httpManager4->get(httpRequest);
        connect(httpManager4,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
            if(httpReply->error()!=QNetworkReply::NoError){
                MessageWidget message;
                message.setText("网络好像断开了","确定","取消");
                message.exec();
            }else{
                //获取响应信息
                const QByteArray data=httpReply->readAll();
                // 解析响应结果
                QJsonParseError jsonError;
                QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));
                if(jsonError.error != QJsonParseError::NoError)return;
                QJsonObject rootObj = jsonDoc.object();
                if(rootObj.value("code").toString()=="0"){
                    MessageWidget message;
                    message.setText(rootObj.value("msg").toString(),"确定","取消");
                    message.exec();
                }
                this->deletedNote();
            }
        });
    }
}
CatalogItem::~CatalogItem()
{
    delete ui;
    delete httpManager1;
    delete httpManager2;
    delete httpManager3;
    delete httpManager4;
}
