#include "widget.h"
#include "ui_widget.h"

Widget::Widget(int userId,QString userName,QString preUrl,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,userId(userId)
    ,userName(userName)
{
    ui->setupUi(this);
    isMove=false;
    catalogId=0;
    this->preUrl=preUrl;
    this->setWindowFlags(Qt::FramelessWindowHint);// 去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);// 设置窗体透明
    QString style="QScrollBar:vertical {border: none;width:8px;margin: 0 0 0 0;border-radius: 4px;}QScrollBar::handle:vertical {background: rgb(100, 100, 100);border-radius: 4px;}QScrollBar::add-page,QScrollBar::sub-page{background: transparent;}QScrollBar::add-line,QScrollBar::sub-line{background: transparent;}";
    ui->privateCatalogue->verticalScrollBar()->setStyleSheet(style);
    ui->shareCatalogue->verticalScrollBar()->setStyleSheet(style);
    ui->textEdit->verticalScrollBar()->setStyleSheet(style);
    ui->textEdit_2->verticalScrollBar()->setStyleSheet(style);
    ui->textEdit_3->verticalScrollBar()->setStyleSheet(style);
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->userNameLabel->setText(userName);
    httpManager1 = new QNetworkAccessManager(this);
    httpManager2 = new QNetworkAccessManager(this);
    httpManager3 = new QNetworkAccessManager(this);
    httpManager4 = new QNetworkAccessManager(this);
    httpManager5 = new QNetworkAccessManager(this);
    httpManager6 = new QNetworkAccessManager(this);
    httpManager7 = new QNetworkAccessManager(this);
    httpManager8 = new QNetworkAccessManager(this);
    noteWidget=new NotesWidget(preUrl);
    ui->scrollArea->setWidget(noteWidget);
    this->userInfoInit();
    connect(ui->minButton,&QToolButton::clicked,this,&Widget::min);
    connect(ui->maxButton,&QToolButton::clicked,this,&Widget::max);
    connect(ui->closeButton,&QToolButton::clicked,this,&Widget::close);
    connect(ui->settingButton,&QToolButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(0);
    });
    connect(ui->userButton,&QToolButton::clicked,this,[=](){
        ui->stackedWidget_2->setCurrentIndex(0);
    });
    connect(ui->personalSettingButton,&QToolButton::clicked,this,[=](){
        ui->stackedWidget_2->setCurrentIndex(1);
    });
    connect(ui->helpButton,&QToolButton::clicked,this,[=](){
        ui->stackedWidget_2->setCurrentIndex(2);
    });
    connect(ui->aboutButton,&QToolButton::clicked,this,[=](){
        ui->stackedWidget_2->setCurrentIndex(3);
    });
    connect(ui->editButton,&QToolButton::clicked,this,[=](){
        ui->lineEdit->setReadOnly(false);
        ui->lineEdit_2->setReadOnly(false);
    });
    connect(ui->logoutButton,&QToolButton::clicked,this,&Widget::logout);
    connect(ui->saveButton,&QToolButton::clicked,this,&Widget::saveInfo);
    connect(ui->deleteUserButton,&QToolButton::clicked,this,&Widget::deleteUser);
    connect(ui->findButton,&QToolButton::clicked,this,[=](){
        this->noteWidget->update(ui->comboBox->currentIndex(),ui->findLineEdit->text());
    });
    connect(ui->findButton_2,&QToolButton::clicked,this,&Widget::newNote);
    connect(noteWidget,&NotesWidget::selected,this,&Widget::findNote);
    connect(ui->putoutButton,&QToolButton::clicked,this,&Widget::download);
    connect(ui->returnButton,&QToolButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->returnButton_2,&QToolButton::clicked,this,&Widget::saveNote);
}
// 成员函数
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(event->pos().x()>0&&event->pos().x()<this->width()&&event->pos().y()>0&&event->pos().y()<ui->frame_5->height()){// 如果鼠标按下的位置在标题栏上
            position=event->globalPos();//获取鼠标绝对位置
            isMove=true;
        }

    }
}
void Widget::mouseMoveEvent(QMouseEvent *event)
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
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    isMove=false;
}
// 工具函数
void Widget::min()
{
    this->setWindowState(Qt::WindowMinimized);
    ui->maxButton->setIcon(QIcon(":/resource/max1.png"));
}
void Widget::max()
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
void Widget::userInfoInit()
{
    findCatalog();
    QFile file("sys/user.info");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    int id;QString userName,password;
    stream>>id>>userName>>password;
    file.close();
    ui->lineEdit->setText(userName);
    ui->lineEdit_2->setText(password);

    QTreeWidgetItem* item3=new QTreeWidgetItem(ui->shareCatalogue);
    items3.push_back(item3);
    CatalogItem* item4=new CatalogItem(0,"共享文档",userId,0,true,false,preUrl);
    connect(item4,&CatalogItem::shareCreated,this,[=](int id,int nid,QString name,int power){
        QTreeWidgetItem* temp3=new QTreeWidgetItem(items3.at(0));
        items3.push_back(temp3);
        CatalogItem* temp4=new CatalogItem(id,name,nid,0,false,false,preUrl,power);
        connect(temp4,&CatalogItem::deletedNote,this,[=](){
            int i=0;
            for(;i<items4.size();i++)if(items4[i]==temp4)break;
            QTreeWidgetItem* t3=items3.at(i);
            items3.removeAt(i);
            delete t3;
            items4.removeAt(i);
            delete temp4;
        });
        connect(temp4,&CatalogItem::selected,this,&Widget::findNote);
        items4.push_back(temp4);
        ui->shareCatalogue->setItemWidget(temp3,0,temp4);
    });
    items4.push_back(item4);
    ui->shareCatalogue->setItemWidget(item3,0,item4);
    findShareNote();
}
void Widget::findCatalog()
{
    QNetworkRequest httpRequest;
    QString fullUrl=preUrl+"/catalog/find?"+"userId="+QString::number(userId);
    httpRequest.setUrl(QUrl(fullUrl));
    httpManager1->get(httpRequest);
    connect(httpManager1,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
        QTreeWidgetItem* item1;
        CatalogItem* item2;
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
            QVariantList list = jsonDoc.toVariant().toList();
            for(int i=0; i<list.size(); i++)
            {
                QVariantMap map = list[i].toMap();
                int id=map["id"].toInt();
                int fatherId=map["fatherId"].toInt();
                QString name=map["name"].toString();
                bool root=true;
                if(fatherId==0)item1=new QTreeWidgetItem(ui->privateCatalogue);
                else{
                    int i=0;
                    for(;i<items2.size();i++)if(fatherId==items2[i]->getId())break;
                    item1=new QTreeWidgetItem(items1[i]);
                    root=false;
                }
                items1.push_back(item1);
                item2=new CatalogItem(id,name,userId,fatherId,root,true,preUrl);
                connect(item2,&CatalogItem::deleted,this,&Widget::deleteCatalog);
                connect(item2,&CatalogItem::created,this,&Widget::newCatalog);
                connect(item2,&CatalogItem::selected,this,&Widget::findNotes);
                connect(item2,&CatalogItem::nameChanged,this,[=](QString name){ui->label->setText(name);});
                items2.push_back(item2);
                ui->privateCatalogue->setItemWidget(item1,0,item2);
            }
            ui->privateCatalogue->expandAll();// 全部展开
        }
    });
}
void Widget::findShareNote(){
    QNetworkRequest httpRequest;
    QString fullUrl=preUrl+"/shareNote/find?"+"sid="+QString::number(userId);
    httpRequest.setUrl(QUrl(fullUrl));
    httpManager8->get(httpRequest);
    connect(httpManager8,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
        QTreeWidgetItem* item3;
        CatalogItem* item4;
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
            QVariantList list = jsonDoc.toVariant().toList();
            for(int i=0; i<list.size(); i++)
            {
                QVariantMap map = list[i].toMap();
                int id=map["id"].toInt();
                QString name=map["noteName"].toString();
                int nid=map["noteId"].toInt();
                int power=map["power"].toInt();
                item3=new QTreeWidgetItem(items3.at(0));
                items3.push_back(item3);
                item4=new CatalogItem(id,name,nid,0,false,false,preUrl,power);
                connect(item4,&CatalogItem::deletedNote,this,[=](){
                    int i=0;
                    for(;i<items4.size();i++)if(items4[i]==item4)break;
                    QTreeWidgetItem* t3=items3.at(i);
                    items3.removeAt(i);
                    delete t3;
                    items4.removeAt(i);
                    delete item4;
                });
                connect(item4,&CatalogItem::selected,this,&Widget::findNote);
                items4.push_back(item4);
                ui->shareCatalogue->setItemWidget(item3,0,item4);
            }
            ui->shareCatalogue->expandAll();// 全部展开
        }
    });
}
void Widget::deleteCatalog(int id)
{
    QVector<QTreeWidgetItem*> temp1;
    QVector<CatalogItem*> temp2;
    help(temp1,temp2,id);
    for(int i=0;i<temp1.size();i++){
        items1.removeOne(temp1[i]);
    }
    delete temp1[0];
    for(int i=0;i<temp2.size();i++){
        items2.removeOne(temp2[i]);
        temp2[i]->deleteLater();
    }
    ui->stackedWidget->setCurrentIndex(0);// 回到设置页
}
void Widget::help(QVector<QTreeWidgetItem*>& temp1,QVector<CatalogItem*>& temp2,int id){
    QVector<int> ids;
    bool flag=false;
    for(int i=0;i<items2.size();i++){
        if(id==items2[i]->getFatherId()){
            flag=true;
            ids.push_back(items2[i]->getId());
        }else if(id==items2[i]->getId()){
            temp1.push_back(items1[i]);
            temp2.push_back(items2[i]);
        }
    }
    if(flag){
        for(int i=0;i<ids.size();i++)help(temp1,temp2,ids[i]);
    }
}
void Widget::newCatalog(int id,int childId)
{
    int i=0;
    for(;i<items2.size();i++){
        if(id==items2[i]->getId())break;
    }
    QTreeWidgetItem* temp1=new QTreeWidgetItem(items1[i]);
    items1.push_back(temp1);
    CatalogItem* temp2=new CatalogItem(childId,"新建文件夹",userId,id,false,true,preUrl);
    connect(temp2,&CatalogItem::deleted,this,&Widget::deleteCatalog);
    connect(temp2,&CatalogItem::created,this,&Widget::newCatalog);
    connect(temp2,&CatalogItem::selected,this,&Widget::findNotes);
    connect(temp2,&CatalogItem::nameChanged,this,[=](QString name){ui->label->setText(name);});
    items2.push_back(temp2);
    ui->privateCatalogue->setItemWidget(temp1,0,temp2);
}
void Widget::logout(){
    bool select;
    MessageWidget message(&select);
    message.setText("确定退出登录？","确定","取消");
    message.exec();
    if(select){
        QFile::remove("sys/user.info");// 删除文件
        this->close();
    }
}
void Widget::saveInfo()
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
        bool select;
        MessageWidget message(&select);
        message.setText("确定保存？","确定","取消");
        message.exec();
        if(select){
            QNetworkRequest httpRequest;
            QString fullUrl=preUrl+"/user/update";
            httpRequest.setUrl(QUrl(fullUrl));
            httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            QJsonObject tempObj;
            tempObj.insert("id",userId);
            tempObj.insert("userName",ui->lineEdit->text());
            tempObj.insert("password",ui->lineEdit_2->text());
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
                        // 修改文件
                        QFile file("sys/user.info");
                        file.open(QIODevice::WriteOnly);
                        QTextStream stream(&file);
                        stream<<userId<<' ';
                        stream<<ui->lineEdit->text()<<' ';
                        stream<<ui->lineEdit_2->text();
                        file.close();
                        ui->lineEdit->setReadOnly(true);
                        ui->lineEdit_2->setReadOnly(true);
                        ui->userNameLabel->setText(ui->lineEdit->text());
                    }
                }
            });
        }
    }
}
void Widget::deleteUser()
{
    bool select;
    MessageWidget message(&select);
    message.setText("确定注销账号？","确定","取消");
    message.exec();
    if(select){
        QNetworkRequest httpRequest;
        QString fullUrl=preUrl+"/user/logout?"+"id="+QString::number(userId);
        httpRequest.setUrl(QUrl(fullUrl));
        httpManager3->get(httpRequest);
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
                    QFile::remove("sys/user.info");// 删除文件
                    this->close();
                }
            }
        });
    }
}
void Widget::findNotes(int id, QString name,int power)
{
    this->catalogId=id;
    ui->stackedWidget->setCurrentIndex(2);
    ui->label->setText(name);
    ui->findLineEdit->setText("");
    ui->comboBox->setCurrentIndex(0);
    // 根据目录的id查询笔记
    QNetworkRequest httpRequest;
    QString fullUrl=preUrl+"/note/find?"+"cid="+QString::number(id);
    httpRequest.setUrl(QUrl(fullUrl));
    httpManager4->get(httpRequest);
    connect(httpManager4,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
        if(httpReply->error()!=QNetworkReply::NoError){
            MessageWidget message;
            message.setText("网络好像断开了","确定","取消");
            message.exec();
        }else{
            QList<int> ids;
            QList<QString> names;
            QList<QString> authors;
            QList<QString> labels;
            QList<QString> shareCodes;
            QList<int> defaultPowers;
            //获取响应信息
            const QByteArray data=httpReply->readAll();
            // 解析响应结果
            QJsonParseError jsonError;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &jsonError));
            if(jsonError.error != QJsonParseError::NoError)return;
            QVariantList list = jsonDoc.toVariant().toList();
            for(int i=0; i<list.size(); i++)
            {
                QVariantMap map = list[i].toMap();
                ids.push_back(map["id"].toInt());
                names.push_back(map["name"].toString());
                authors.push_back(map["author"].toString());
                labels.push_back(map["label"].toString());
                shareCodes.push_back(map["shareCode"].toString());
                defaultPowers.push_back(map["defaultPower"].toInt());
            }
            this->noteWidget->init(ids.size());
            this->noteWidget->update(ids,names,authors,labels,shareCodes,defaultPowers,true);
        }
    });
}
void Widget::newNote()
{
    QNetworkRequest httpRequest;
    QString fullUrl=preUrl+"/note/create";
    httpRequest.setUrl(QUrl(fullUrl));
    httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QJsonObject tempObj;
    tempObj.insert("name","新建笔记");
    tempObj.insert("author",userName);
    tempObj.insert("label","#");
    tempObj.insert("shareCode","0");
    tempObj.insert("defaultPower",0);
    tempObj.insert("userId",userId);
    tempObj.insert("catalogId",catalogId);
    QJsonDocument tempDoc;
    tempDoc.setObject(tempObj);
    QByteArray data = tempDoc.toJson(QJsonDocument::Compact);
    httpManager5->post(httpRequest,data);
    connect(httpManager5,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
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
            if(rootObj.value("code").toString()=="0"){// 增加一个笔记标签
                QJsonObject childObj=rootObj.value("data").toObject();
                this->noteWidget->create(childObj.value("id").toInt(),"新建笔记",userName,"#",childObj.value("shareCode").toString(),0);
                ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());// 滚动条自动置底
            }
        }
    });
}
void Widget::findNote(int id,QString name,int power){
    ui->stackedWidget->setCurrentIndex(3);
    ui->label_4->setText(name);
    this->noteId=id;
    this->cpower=power;
    QNetworkRequest httpRequest;
    QString fullUrl=preUrl+"/note/findText?"+"nid="+QString::number(id);
    httpRequest.setUrl(QUrl(fullUrl));
    httpManager6->get(httpRequest);
    connect(httpManager6,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
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
                ui->textEdit->setText(rootObj.value("data").toString());
                if(this->cpower==0){
                    ui->textEdit->setReadOnly(true);
                    ui->returnButton_2->setEnabled(false);
                }else{
                    ui->textEdit->setReadOnly(false);
                    ui->returnButton_2->setEnabled(true);
                }
            }
        }
    });
}
void Widget::saveNote(){
    QNetworkRequest httpRequest;
    QString fullUrl=preUrl+"/note/updateText";
    httpRequest.setUrl(QUrl(fullUrl));
    httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QJsonObject tempObj;
    tempObj.insert("noteId",this->noteId);
    tempObj.insert("text",ui->textEdit->toPlainText());
    QJsonDocument tempDoc;
    tempDoc.setObject(tempObj);
    QByteArray data = tempDoc.toJson(QJsonDocument::Compact);
    httpManager7->post(httpRequest,data);
    connect(httpManager7,&QNetworkAccessManager::finished,this,[=](QNetworkReply *httpReply){
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
            if(rootObj.value("code").toString()=="0"){// 增加一个笔记标签
                MessageWidget message;
                message.setText("保存成功","确定","取消");
                message.exec();
            }
        }
    });
}
void Widget::download(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("导出笔记"),"",tr("PDF文件(*pdf)"));// 只能选择一些指定的文件类型
    if(fileName!=""){
        QFile pdfFile(fileName+".pdf");
        pdfFile.open(QIODevice::WriteOnly);
        QPdfWriter *pdfWriter = new QPdfWriter(&pdfFile);
        pdfWriter->setPageSize(QPagedPaintDevice::A4);
        pdfWriter->setResolution(QPrinter::ScreenResolution);
        pdfWriter->setPageMargins(QMarginsF(40, 40, 40, 40));
        QPainter *pdfPainter = new QPainter(pdfWriter);
        //设置标题
        QTextOption option(Qt::AlignCenter);//标题居中显示
        option.setWrapMode(QTextOption::WordWrap);//标题自动换行
        pdfPainter->setFont(QFont("楷体",22,QFont::Bold));
        pdfPainter->drawText(QRect(0, 0, 8500, 450),ui->label_4->text(),option);
        //设置内容
        option.setAlignment(Qt::AlignLeft);
        pdfPainter->setFont(QFont("楷体",14));
        QStringList texts=ui->textEdit->toPlainText().split('\n');
        int counter=0;
        for(QStringList::iterator iter=texts.begin();iter!=texts.end();iter++){
            if((*iter).size()<=36){pdfPainter->drawText(QRect(0, 620+counter*300, 8500, 300),*iter, option);}
            else{
                QStringList temp;
                int s=qCeil((*iter).size()/36.0);
                for(int i=0;i<s;i++){
                    temp.push_back((*iter).mid(i*36,36));
                }
                for(QStringList::iterator iter1=temp.begin();iter1!=temp.end();iter1++){
                    pdfPainter->drawText(QRect(0, 620+counter*300, 8500, 300),*iter1, option);
                    counter++;
                }
            }
            counter++;
        }
        delete pdfPainter;
        delete pdfWriter;
        pdfFile.close();
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName+".pdf"));
    }
}
Widget::~Widget()
{
    delete ui;
    delete httpManager1;
    delete httpManager2;
    delete httpManager3;
    delete httpManager4;
    delete httpManager5;
    delete httpManager6;
    delete httpManager7;
    delete httpManager8;
    delete noteWidget;
    for(int i=0;i<items1.size();i++)delete items1[i];
    for(int i=0;i<items2.size();i++)delete items2[i];
    for(int i=0;i<items3.size();i++)delete items3[i];
    for(int i=0;i<items4.size();i++)delete items4[i];
    items1.clear();
    items2.clear();
    items3.clear();
    items4.clear();
}

