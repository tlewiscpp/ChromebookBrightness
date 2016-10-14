#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QFocusEvent>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    CustomLineEdit(QWidget *parent);
    ~CustomLineEdit();

signals:
    void focusGained();
    void focusLost();
    
private:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

};

#endif //CUSTOMLINEEDIT_H
