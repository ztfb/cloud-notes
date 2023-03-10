#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QDialog>
#include <QFocusEvent>
namespace Ui {
class MessageWidget;
}

class MessageWidget : public QDialog
{
    Q_OBJECT

public:
    explicit MessageWidget(bool* select=nullptr,QWidget *parent = nullptr);
    void setText(QString text1,QString text2,QString text3);
    ~MessageWidget();
private:
    Ui::MessageWidget *ui;
};

#endif // MESSAGEWIDGET_H
