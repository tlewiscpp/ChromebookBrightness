#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent = 0)
{
    this->setParent(parent);
}

void CustomLineEdit::focusInEvent(QFocusEvent *e) 
{
    //this->setStyleSheet("background: white;");
    QLineEdit::focusInEvent(e);
    this->setStyleSheet("border: 1px solid transparent; background: transparent;");
    emit(focusGained());
}

void CustomLineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    this->setStyleSheet("border: 1px solid transparent; background: transparent;");
    emit(focusLost());
}

CustomLineEdit::~CustomLineEdit()
{

}
