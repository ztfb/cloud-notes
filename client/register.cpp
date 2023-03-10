#include "register.h"
#include "ui_register.h"

Register::Register(QString preUrl,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    isMove=false;
    this->preUrl=preUrl;
    this->setWindowFlags(Qt::FramelessWindowHint);// 去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);// 设置窗体透明
    ui->stackedWidget->setCurrentIndex(0);
    httpManager1 = new QNetworkAccessManager(this);
    httpManager2 = new QNetworkAccessManager(this);
    connect(ui->minButton,&QToolButton::clicked,this,&Register::min);
    connect(ui->maxButton,&QToolButton::clicked,this,&Register::max);
    connect(ui->closeButton,&QToolButton::clicked,this,&Register::close);
    connect(ui->loginButton,&QToolButton::clicked,this,[=](){
        if(ui->stackedWidget->currentIndex()==1)ui->stackedWidget->setCurrentIndex(0);
        else this->login();
    });
    connect(ui->regButton,&QToolButton::clicked,this,[=](){
        if(ui->stackedWidget->currentIndex()==0)ui->stackedWidget->setCurrentIndex(1);
        else this->reg();
    });
}
// 成员函数
void Register::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(event->pos().x()>0&&event->pos().x()<this->width()&&event->pos().y()>0&&event->pos().y()<ui->frame_5->height()){// 如果鼠标按下的位置在标题栏上
            position=event->globalPos();//获取鼠标绝对位置
            isMove=true;
        }

    }
}
void Register::mouseMoveEvent(QMouseEvent *event)
{
    if(isMove){//当鼠标按下时才能拖拽
        QFlags<Qt::WindowState>::Int state=this->windowState();
        if(state==Qt::WindowMaximized){// 最大化状态
            this->setWindowState(Qt::WindowNoState);// 改变状态
            ui->maxButton->setIcon(QIcon(":/resource/max1.png"));// 改变图标样式
        }
        QPoint p=event->globalPos();//获取临时位置
        //调整窗口位置
        this->move(this->pos().x()+p.x()-position.x(),this->pos().y()+p.y()-position.y());
        position=p;//更新位置
    }
}
void Register::mouseReleaseEvent(QMouseEvent *event)
{
    isMove=false;
}
// 工具函数
void Register::min()
{
    this->setWindowState(Qt::WindowMinimized);
    ui->maxButton->setIcon(QIcon(":/resource/max1.png"));
}
void Register::max()
{
    QFlags<Qt::WindowState>::Int state=this->windowState();
    if(state==Qt::WindowMaximized){// 最大化状态
        this->setWindowState(Qt::WindowNoState);// 改变状态
        ui->maxButton->setIcon(QIcon(":/resource/max1.png"));// 改变图标样式
    }else if(state==Qt::WindowNoState){
        this->setWindowState(Qt::WindowMaximized);
        ui->maxButton->setIcon(QIcon(":/resource/max2.png"));
    }
}
void Register::login()
{
    if(ui->lineEdit->text()==""){
        MessageWidget message;
        message.setText("请输入用户名","确定","取消");
        message.exec();
    }else if(ui->lineEdit_2->text()==""){
        MessageWidget message;
        message.setText("请输入密码","确定","取消");
        message.exec();
    }else{// 向后端发起请求
        QNetworkRequest httpRequest;
        QString fullUrl=preUrl+"/user/login?"+"userName="+ui->lineEdit->text()+"&password="+ui->lineEdit_2->text();
        httpRequest.setUrl(QUrl(fullUrl));
        httpManager1->get(httpRequest);
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
                    // 将用户信息写入文件
                    QFile file("sys/user.info");
                    file.open(QIODevice::WriteOnly);
                    QTextStream stream(&file);
                    stream<<childObj.value("id").toInt()<<' ';
                    stream<<childObj.value("userName").toString()<<' ';
                    stream<<childObj.value("password").toString();
                    file.close();
                    MessageWidget message;
                    message.setText(rootObj.value("msg").toString(),"确定","取消");
                    message.exec();
                    // 进入软件主界面
                    Widget* w=new Widget(childObj.value("id").toInt(),childObj.value("userName").toString(),preUrl);
                    w->setAttribute(Qt::WA_DeleteOnClose);
                    w->show();
                    this->close();
                }else{
                    MessageWidget message;
                    message.setText(rootObj.value("msg").toString(),"确定","取消");
                    message.exec();
                }
            }
        });
    }
}
void Register::reg()
{
    if(ui->lineEdit_3->text()==""){
        MessageWidget message;
        message.setText("请输入用户名","确定","取消");
        message.exec();
    }else if(ui->lineEdit_4->text()==""){
        MessageWidget message;
        message.setText("请输入密码","确定","取消");
        message.exec();
    }else if(ui->lineEdit_5->text()==""){
        MessageWidget message;
        message.setText("请输入密码","确定","取消");
        message.exec();
    }else if(ui->lineEdit_4->text()!=ui->lineEdit_5->text()){
        MessageWidget message;
        message.setText("两次密码不一致","确定","取消");
        message.exec();
    }else{// 向后端发起请求
        QNetworkRequest httpRequest;
        QString fullUrl=preUrl+"/user/register";
        httpRequest.setUrl(QUrl(fullUrl));
        httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        QJsonObject tempObj;
        tempObj.insert("userName",ui->lineEdit_3->text());
        tempObj.insert("password",ui->lineEdit_4->text());
        QJsonDocument tempDoc;
        tempDoc.setObject(tempObj);
        QByteArray data = tempDoc.toJson(QJsonDocument::Compact);
        httpManager2->post(httpRequest,data);
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
                    ui->lineEdit_3->setText("");ui->lineEdit_4->setText("");ui->lineEdit_5->setText("");
                    ui->stackedWidget->setCurrentIndex(0);
                }else{
                    MessageWidget message;
                    message.setText(rootObj.value("msg").toString(),"确定","取消");
                    message.exec();
                }
            }
        });
    }
}
Register::~Register()
{
    delete ui;
    delete httpManager1;
    delete httpManager2;
}
