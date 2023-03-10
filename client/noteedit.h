#ifndef NOTEEDIT_H
#define NOTEEDIT_H

#include <QDialog>
#include <messagewidget.h>
namespace Ui {
class NoteEdit;
}

class NoteEdit : public QDialog
{
    Q_OBJECT

public:
    explicit NoteEdit(int id,QString name,QString author,QString label,QWidget *parent = nullptr);
    ~NoteEdit();

private:
    Ui::NoteEdit *ui;
signals:
    void change(int id,QString name,QString author,QString label);
};

#endif // NOTEEDIT_H
