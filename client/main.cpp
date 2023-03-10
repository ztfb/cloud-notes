#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonObject>
#include <QtNetwork>
#include <QByteArray>
#include <register.h>
#include <widget.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopServices::openUrl(QUrl::fromLocalFile("C:\\Users\\HP\\Desktop\\LifeGame"));
    QDir *dir = new QDir;
    if(!dir->exists("sys"))dir->mkdir("sys");// 系统文件目录
    // 网络自动配置
    // 候选ip:依次是本机，内网，公网
    QStringList ips={"127.0.0.1","10.208.78.163"};
    QString port="9090";
    bool* test=new bool[ips.size()];
    for(int i=0;i<ips.size();i++)test[i]=false;
    for(int i=0;i<ips.size();i++){
        QNetworkRequest httpRequest;
        QString preUrl="http://"+ips[i]+":"+port;
        QString fullUrl=preUrl+"/test";
        httpRequest.setUrl(QUrl(fullUrl));
        QNetworkAccessManager httpManager;
        QNetworkReply *httpReply= httpManager.get(httpRequest);
        QEventLoop eventLoop;
        QObject::connect(httpReply, SIGNAL(finished()),&eventLoop, SLOT(quit()));
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
        //对请求的返回异常进行处理
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
            if(jsonError.error != QJsonParseError::NoError)return -1;
            QJsonObject rootObj = jsonDoc.object();
            if(rootObj.value("code").toString()=="0")test[i]=true;
        }
    }
    int i=0;
    for(;ips.size();i++)if(test[i]==true)break;
    QFile file("sys/user.info");// 用户登录状态
    if(!file.exists()){
        // 未登录转入登录界面
        Register r("http://"+ips[i]+":"+port);
        r.show();
        return a.exec();
    }else{
        QFile file("sys/user.info");
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        int id;QString userName;
        stream>>id>>userName;
        file.close();
        // 已登录转入软件界面
        Widget w(id,userName,"http://"+ips[i]+":"+port);
        w.show();
        return a.exec();
    }
}
