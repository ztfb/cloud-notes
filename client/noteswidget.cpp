#include "noteswidget.h"

NotesWidget::NotesWidget(QString preUrl,QWidget *parent) : QWidget(parent)
{
    httpManager1=new QNetworkAccessManager(this);
    httpManager2=new QNetworkAccessManager(this);
    this->preUrl=preUrl;
}
void NotesWidget::init(int num){// 仅仅是创建这些窗口，不对窗口的样式位置做任何调整
    for(QList<NoteItem*>::iterator iter=noteItems.begin();iter!=noteItems.end();iter++){
        delete *iter;
    }
    noteItems.clear();
    ids.clear();
    names.clear();
    authors.clear();
    labels.clear();
    shareCodes.clear();
    defaultPowers.clear();
    for(int counter=0;counter<num;counter++){
        NoteItem* temp=new NoteItem(preUrl,this);
        this->noteItems.append(temp);
        connect(temp,&NoteItem::selected,this,&NotesWidget::selected);
        connect(temp,&NoteItem::deleted,this,&NotesWidget::deleteItem);
        connect(temp,&NoteItem::change,this,&NotesWidget::changeItem);
    }
    showNum=noteItems.size();
}
void NotesWidget::sizeAndPositionUpdate(){// 当前窗口的大小发生变化时，需要调用该函数重新计算各个标签位置
    int w=qRound(cw*0.99);// 宽
    int h=qRound(cw*0.1);// 高
    int s=qRound(cw*0.005);
    NoteItem* temp;
    int counter=0;
    for(;counter<showNum;counter++){
        temp=noteItems.at(counter);
        temp->resize(w,h);
        temp->move(s,s+counter*(h+s));
    }
    this->setMinimumHeight(s+counter*(h+s));
}
void NotesWidget::create(int id,QString name,QString author,QString label,QString shareCode,int defaultPower){
    if(ids.size()==noteItems.size()){
        NoteItem* temp=new NoteItem(preUrl,this);
        connect(temp,&NoteItem::selected,this,&NotesWidget::selected);
        connect(temp,&NoteItem::deleted,this,&NotesWidget::deleteItem);
        connect(temp,&NoteItem::change,this,&NotesWidget::changeItem);
        this->noteItems.append(temp);
    }
    this->ids.append(id);
    this->names.append(name);
    this->authors.append(author);
    this->labels.append(label);
    this->shareCodes.append(shareCode);
    this->defaultPowers.append(defaultPower);
    this->update(ids,names,authors,labels,shareCodes,defaultPowers,false);
}
void NotesWidget::remove(int id){
    int index=0;
    for(;index<ids.size();index++)if(id==ids[index])break;
    this->ids.removeAt(index);
    this->names.removeAt(index);
    this->authors.removeAt(index);
    this->labels.removeAt(index);
    this->shareCodes.removeAt(index);
    this->defaultPowers.removeAt(index);
    this->update(ids,names,authors,labels,shareCodes,defaultPowers,false);
}
void NotesWidget::update(QList<int>ids,QList<QString> names,QList<QString> authors,QList<QString> labels,QList<QString> shareCodes,QList<int> defaultPowers,bool flag){
    if(flag){
        this->ids=ids;
        this->names=names;
        this->authors=authors;
        this->labels=labels;
        this->shareCodes=shareCodes;
        this->defaultPowers=defaultPowers;
    }
    for(QList<NoteItem*>::iterator iter=noteItems.begin();iter!=noteItems.end();iter++)(*iter)->hide();// 隐藏
    QList<NoteItem*>::iterator iter=noteItems.begin();
    QList<int>::iterator iter1=ids.begin();
    QList<QString>::iterator iter2=names.begin();
    QList<QString>::iterator iter3=authors.begin();
    QList<QString>::iterator iter4=labels.begin();
    QList<QString>::iterator iter5=shareCodes.begin();
    QList<int>::iterator iter6=defaultPowers.begin();
    for(;iter!=noteItems.end()&&iter1!=ids.end();iter++,iter1++,iter2++,iter3++,iter4++,iter5++,iter6++){
        (*iter)->reAssign((*iter1),(*iter2),(*iter3),(*iter4),(*iter5),(*iter6));
        (*iter)->update();
        (*iter)->show();
    }
    showNum=ids.size();
    this->sizeAndPositionUpdate();// 重新计算各个标签的位置
}
void NotesWidget::update(int index, QString text)
{
    QList<int>idsT;
    QList<QString> namesT;
    QList<QString> authorsT;
    QList<QString> labelsT;
    QList<QString> shareCodesT;
    QList<int> defaultPowersT;
    QList<int>::iterator iter1=ids.begin();
    QList<QString>::iterator iter2=names.begin();
    QList<QString>::iterator iter3=authors.begin();
    QList<QString>::iterator iter4=labels.begin();
    QList<QString>::iterator iter5=shareCodes.begin();
    QList<int>::iterator iter6=defaultPowers.begin();
    for(;iter1!=ids.end();iter1++,iter2++,iter3++,iter4++,iter5++,iter6++){
        switch (index) {
        case 0:{
            if((*iter2).contains(text)||text==""){
                idsT.push_back((*iter1));
                namesT.push_back((*iter2));
                authorsT.push_back((*iter3));
                labelsT.push_back((*iter4));
                shareCodesT.push_back((*iter5));
                defaultPowersT.push_back((*iter6));
            }
            break;
        }
        case 1:{
            if((*iter3).contains(text)||text==""){
                idsT.push_back((*iter1));
                namesT.push_back((*iter2));
                authorsT.push_back((*iter3));
                labelsT.push_back((*iter4));
                shareCodesT.push_back((*iter5));
                defaultPowersT.push_back((*iter6));
            }
            break;
        }
        case 2:{
            if((*iter4).contains(text)||text==""){
                idsT.push_back((*iter1));
                namesT.push_back((*iter2));
                authorsT.push_back((*iter3));
                labelsT.push_back((*iter4));
                shareCodesT.push_back((*iter5));
                defaultPowersT.push_back((*iter6));
            }
            break;
        }
        }
    }
    this->update(idsT,namesT,authorsT,labelsT,shareCodesT,defaultPowersT,false);
}
void NotesWidget::resizeEvent(QResizeEvent *event)
{
    cw=this->width();
    ch=this->width();
    this->sizeAndPositionUpdate();// 重新计算位置
}
void NotesWidget::deleteItem(int id){
    this->cid=id;
    // 删除选中的项
    bool select;
    MessageWidget message(&select);
    message.setText("确定删除？","确定","取消");
    message.exec();
    if(select){
        QNetworkRequest httpRequest;
        QString fullUrl=preUrl+"/note/delete?"+"id="+QString::number(id);
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
                if(jsonError.error != QJsonParseError::NoError)return;
                QJsonObject rootObj = jsonDoc.object();
                if(rootObj.value("code").toString()=="0"){
                    MessageWidget message;
                    message.setText(rootObj.value("msg").toString(),"确定","取消");
                    message.exec();
                    this->remove(this->cid);
                }
            }
        });
    }
}
void NotesWidget::changeItem(int id, QString name, QString author, QString label){
    bool select=false;
    MessageWidget message(&select);
    message.setText("确定保存？","确定","取消");
    message.exec();
    if(select){
        this->cid=id;this->cname=name;this->cauthor=author;this->clabel=label;
        QNetworkRequest httpRequest;
        QString fullUrl=preUrl+"/note/update";
        httpRequest.setUrl(QUrl(fullUrl));
        httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        QJsonObject tempObj;
        tempObj.insert("id",id);
        tempObj.insert("name",name);
        tempObj.insert("author",author);
        tempObj.insert("label",label);
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
                    int index=0;
                    for(;index<ids.size();index++)if(cid==ids[index])break;
                    names[index]=cname;
                    authors[index]=cauthor;
                    labels[index]=clabel;
                    this->update(ids,names,authors,labels,shareCodes,defaultPowers,false);
                }
            }
        });
    }
}
NotesWidget::~NotesWidget(){
    for(QList<NoteItem*>::iterator iter=noteItems.begin();iter!=noteItems.end();iter++){
        delete *iter;
    }
    delete httpManager1;
    delete httpManager2;
}
