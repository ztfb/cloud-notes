#include "sharewidget.h"
#include "ui_sharewidget.h"

ShareWidget::ShareWidget(int id,QString shareCode,int defaultPower,QString preUrl,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareWidget)
{
    ui->setupUi(this);
    this->id=id;this->shareCode=shareCode;this->defaultPower=defaultPower;this->preUrl=preUrl;
    this->setWindowFlags(Qt::FramelessWindowHint);// 去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);// 设置窗体透明
    httpManager1 = new QNetworkAccessManager(this);
    if(defaultPower==0)ui->radioButton->setChecked(true);
    else if(defaultPower==1)ui->radioButton_2->setChecked(true);
    connect(ui->toolButton_2,&QToolButton::clicked,this,&ShareWidget::close);
    connect(ui->toolButton,&QToolButton::clicked,this,[=](){
        QClipboard* board = QApplication::clipboard();
        board->setText(this->shareCode);
        MessageWidget message;
        message.setText("复制成功，快去分享给好友吧","确定","取消");
        message.exec();
    });
    connect(ui->returnButton_2,&QToolButton::clicked,this,[=](){
        int cpower=0;
        if(ui->radioButton->isChecked())cpower=0;
        if(ui->radioButton_2->isChecked())cpower=1;
        if(cpower!=this->defaultPower){
            bool select=false;
            MessageWidget message(&select);
            message.setText("确定更改权限吗？","确定","取消");
            message.exec();
            if(select){// 更改权限
                QNetworkRequest httpRequest;
                QString fullUrl=preUrl+"/note/updatePower";
                httpRequest.setUrl(QUrl(fullUrl));
                httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
                QJsonObject tempObj;
                tempObj.insert("id",this->id);
                tempObj.insert("defaultPower",cpower);
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
                        if(jsonError.error != QJsonParseError::NoError)return;
                        QJsonObject rootObj = jsonDoc.object();
                        if(rootObj.value("code").toString()=="0"){
                            this->defaultPower=cpower;
                            this->powerChanged(this->defaultPower);
                            MessageWidget message;
                            message.setText(rootObj.value("msg").toString(),"确定","取消");
                            message.exec();
                        }
                    }
                });
            }
        }
    });
}

ShareWidget::~ShareWidget()
{
    delete ui;
    delete httpManager1;
}
