#ifndef MYBTN_H
#define MYBTN_H

#include <QPushButton>

class myBtn : public QPushButton
{
public:
    myBtn(QWidget *parent=0);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MYBTN_H
