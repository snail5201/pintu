#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLabel>
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void cutimage();//九宫格分割

    void Random();//打乱

    void moveImage();//图片移动

    void mousePressEvent(QMouseEvent *event);//鼠标点击事件

    void keyPressEvent(QKeyEvent *event);//键盘事件

    int panduanwancheng();//判断完成

    void huifu();//在判填充原来挖空的地方

    int bushusum;//这个变量储存总步数；

    float score;

    QTimer *timer;

    int tim;

private slots:      //在槽函数中 只能包含函数

    void on_btn_clicked();

    void onTimerOut();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QString strFileName;//文件名称

    QImage* pSourceImage;//原图

    QLabel* pLbImage[9];//九宫格label

    QImage pImage[3][3];

    int pCompare[3][3];  //标记数组；

    //QImage QFrame;
};

#endif // MAINWINDOW_H
