#include "mybtn.h"
#include <QKeyEvent>
myBtn::myBtn(QWidget *parent):QPushButton(parent)
{

}

void myBtn::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}


