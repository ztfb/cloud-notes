#include "newsharenote.h"
#include "ui_newsharenote.h"

NewShareNote::NewShareNote(QString preUrl,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewShareNote)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);// 去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);// 设置窗体透明
    httpManager1=new QNetworkAccessManager(this);
    connect(ui->toolButton_2,&QToolButton::clicked,this,&NewShareNote::close);
    connect(ui->toolButton,&QToolButton::clicked,this,[=](){
        if(ui->lineEdit->text()==""){
            MessageWidget message;
            message.setText("未输入共享码","确定","取消");
            message.exec();
        }else{
            QFile file("sys/user.info");
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            int id;stream>>id;
            file.close();
            QNetworkRequest httpRequest;
            QString fullUrl=preUrl+"/note/shareNew";
            httpRequest.setUrl(QUrl(fullUrl));
            httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            QJsonObject tempObj;
            tempObj.insert("userId",id);
            tempObj.insert("shareCode",ui->lineEdit->text());
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
                    //获取响应信息
                    const QByteArray data=httpReply->readAll();
                    // 解析响应结果
                    QJsonParseError jsonError;
                    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));
                    if(jsonError.error != QJsonParseError::NoError)return ;
                    QJsonObject rootObj = jsonDoc.object();
                    if(rootObj.value("code").toString()=="0"){
                        QJsonObject childObj=rootObj.value("data").toObject();
                        this->shareCreated(childObj.value("id").toInt(),childObj.value("nid").toInt(),childObj.value("name").toString(),childObj.value("power").toInt());
                        MessageWidget message;
                        message.setText(rootObj.value("msg").toString(),"确定","取消");
                        message.exec();
                    }else{
                        MessageWidget message;
                        message.setText(rootObj.value("msg").toString(),"确定","取消");
                        message.exec();
                    }
                }
            });
        }
    });
}

NewShareNote::~NewShareNote()
{
    delete ui;
    delete httpManager1;
}
