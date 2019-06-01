#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <QMouseEvent>
#include <QMessageBox>
#include <QtGlobal>//随意数头文件
#include <QTimer>
#include <QTime>
#include "mybtn.h"
#define SMALL_W 130//每个小控件的宽和高
#define SMALL_H 100
#define PHOTO_X 20//3*3布局左上角坐标（20,110）
#define PHOTO_Y 110

//构造函数
MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer(this);     //定时器
    connect(timer,SIGNAL(timeout()),
            SLOT(onTimerOut()));    //超时信号到达后，执行函数onTimerOut()
    bushusum=0;
    score=0.0;
    tim=0;
    //将int型转化为QString变量，并显示在相应的label中
    ui->label_3->setText((QString::number(bushusum)));
    ui->time_label->setText((QString::number(tim)));
    ui->score_label->setText((QString::number(score)));
    this->setStyleSheet(
     "MainWindow{border-image: url(:/new/prefix1/lx.jpg);}");//增加背景图
    pSourceImage=NULL;//制空
    for(int i=0;i<3;i++){       //将图片划分区域；
        for(int j=0;j<3;j++){
            pLbImage[i*3+j] = new QLabel(this);
            pLbImage[i*3+j]->setGeometry(0,0,SMALL_W,SMALL_H);//显示界面
            pLbImage[i*3+j]->move(PHOTO_X+SMALL_W*i,
                                 PHOTO_Y+SMALL_H*j);//移动label
            pLbImage[i*3+j]->setFrameShape(QFrame::Box);//使label有边框
        }
    }
    connect(ui->btn,SIGNAL(clicked()),this,SLOT(test()));  //按键 信号(有按键按下) 链接 槽函数
}
//析构函数
MainWindow::~MainWindow()
{
    delete ui;
}
//新建按钮
void MainWindow::on_btn_clicked()
{
    //打开对话窗
    strFileName =
        QFileDialog::getOpenFileName(this,
                                     "select pictures",
                                     "/home/newcapec",
                                     "Images(*.png *.jpg)");
    if(strFileName==NULL) return ;
    if(NULL!=pSourceImage)      //把原来的原来的空间释放掉
    {
        delete pSourceImage;
        pSourceImage = NULL;
    }
    pSourceImage =new QImage(strFileName);
    if(pSourceImage == NULL) return ;
    //改变窗口组件的情况下改变窗口背景图片的大小
    QImage tep=pSourceImage ->scaled(ui->label->width(),
                                     ui->label->height());
    //在QLabel上绘制图片
    ui->label->setPixmap(QPixmap::fromImage(tep));
    cutimage();
}
//时间
void MainWindow::onTimerOut()
{
    tim++;
    ui->time_label->setText((QString::number(tim)));
}
//九宫格分割图片
void MainWindow::cutimage()
{
    QImage temp = pSourceImage->scaled(SMALL_W*3,
                                       SMALL_H*3);//缩放图片
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            pImage[i][j]=temp.copy(i*SMALL_W,  //复制图片开始的坐标;
                                   j*SMALL_H,
                                            SMALL_W,
                                            SMALL_H);//复制图片的大小
            pLbImage[i*3+j]->setPixmap(QPixmap::fromImage(pImage[i][j])); //加载图片
            pCompare[i][j]=i*3+j;
        }
    }
    //使右下角为blank.JPG
    QPixmap tep(":/new/prefix1/blank.JPG");
    pLbImage[8]->setPixmap(tep);
    Random();
}
//图片随机打乱
void MainWindow::Random()
{
    bushusum=0;
    score=0.0;
    ui->score_label->setText((QString::number(score)));
    ui->label_3->setText((QString::number(bushusum)));
    tim=0;
    ui->time_label->setText((QString::number(tim)));;
    int x=-1;
    int y=-1;
    for(int w=0;w<3;w++){//找到空白格
        for(int j=0;j<3;j++){
            if(pCompare[w][j]==8){
                x=w;
                y=j;
                break;
            }
        }
        if(x!=-1)
            break;
    }

    if(x==-1||y==-1)
         return ;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//qsrand用来设置一个种子，该种子为qrand生成随机数的起始值。如果不用qsrand设置种子会导致每次运行程序得到的随机数一样
    for(int i=0;i<1000;i++){
        int direction = qrand()%4;  //随机生成0到4的随机数
        switch (direction) {
        case 0://右
            if(x<2){
                pCompare[x][y]=pCompare[x+1][y];
                pCompare[x+1][y]=8;
                x++;
            }
            break;
        case 1://左
            if(x>0){
                pCompare[x][y]=pCompare[x-1][y];
                pCompare[x-1][y]=8;
                x--;
            }
            break;
        case 2://下
            if(y<2){
                pCompare[x][y]=pCompare[x][y+1];
                pCompare[x][y+1]=8;
                y++;
            }
            break;
        case 3://上
            if(y>0){
                pCompare[x][y]=pCompare[x][y-1];
                pCompare[x][y-1]=8;
                y--;
            }
            break;
        default:
            break;
        }
    }
    moveImage();
    if(timer->isActive())
    {
        timer->stop();
    }
    timer->start(1000);
}
//移动图片
void MainWindow::moveImage()//图片移动
{
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            int index = pCompare[i][j];
            pLbImage[index]->move(PHOTO_X+i*SMALL_W,
                                  PHOTO_Y+j*SMALL_H);
        }
    }
}
//判断图片是否拼成
int MainWindow::panduanwancheng ()
{
    int y=1;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(pCompare[i][j]!=i*3+j)
            {
                y=0;
                break;
            }
        }
        if(!y) break;
    }
    return y;
}
//鼠标事件
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int num=0;
    if(pSourceImage==NULL) return ;
    if(panduanwancheng()) return ;
    if(event->button() == Qt::LeftButton    //返回产生事件的按钮
            ||event->button() == Qt::RightButton)
    {
        QPoint pressPoint = event->pos();//获取鼠标点击的位置
        if(pressPoint.x()>PHOTO_X&&pressPoint.y()>PHOTO_Y
                &&pressPoint.x()<PHOTO_X+SMALL_W*3&&pressPoint.y()<PHOTO_Y+SMALL_H*3)
        {
            int x = (pressPoint.x()-PHOTO_X)/SMALL_W;
            int y = (pressPoint.y()-PHOTO_Y)/SMALL_H;

            //判断向右移
            if(x>0&&pCompare[x-1][y]==8)//不是第一行，且他的左面是空白格
            {
                pCompare[x-1][y]=pCompare[x][y];
                pCompare[x][y]=8;
                num++;
            }else if(x<2&&pCompare[x+1][y]==8)//判断向左移
            {
                pCompare[x+1][y]=pCompare[x][y];
                pCompare[x][y]=8;
                num++;

            }else if(y>0&&pCompare[x][y-1]==8)//判断向下移
            {
                pCompare[x][y-1]=pCompare[x][y];
                pCompare[x][y]=8;
                num++;
            }else if(y<2 && pCompare[x][y+1] == 8)//判断向上移动
            {
                pCompare[x][y+1] = pCompare[x][y];
                pCompare[x][y] = 8;
                num++;
            }
        }
    }
    bushusum+=num;
    if(bushusum>1000)
        return ;
    else
    {
    score=100-bushusum/10.0;
    ui->label_3->setText((QString::number(bushusum)));
    ui->score_label->setText((QString::number(score)));
    moveImage();//图片移动
        if(panduanwancheng())
        {
            timer->stop();
            huifu();
        }
    }
}
//拼图完成
void MainWindow::huifu ()
{
    pLbImage[8]->setPixmap(QPixmap::fromImage(pImage[2][2])); //加载图片
    //about()函数是没有关于button设置的按钮的
    QMessageBox::about(this,"successful","***************************************");
}
//退出游戏函数
void MainWindow::on_pushButton_clicked()
{
    if (!(QMessageBox::information(this,tr("Quit Game"),tr("Do you really want to quit the game?"),tr("Yes"),tr("No"))))
    {
        close();
    }
}
//重来函数
void MainWindow::on_pushButton_2_clicked()
{

    if(pSourceImage==NULL) return ;
    QPixmap tep("D:\\aaaaaaaa\\aaa\\blank.jpg");//挖空
    pLbImage[8]->setPixmap(tep);
    Random();
}
//键盘事件
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(pSourceImage==NULL) return ;
    if(panduanwancheng()) return ;
    int x=-1;
    int y=-1;
    for(int w=0;w<3;w++){//找到空白格
        for(int j=0;j<3;j++){
            if(pCompare[w][j]==8){
                x=w;
                y=j;
                break;
            }
        }
        if(x!=-1)
            break;
    }
    if(x==-1||y==-1)
         return ;
    switch(event->key())
    {
    case Qt::Key_Up:
        if(y>0){
            pCompare[x][y]=pCompare[x][y-1];
            pCompare[x][y-1]=8;
            y--;
            moveImage();
            bushusum++;
            if(bushusum>1000)
                return ;
            else
            {
            ui->label_3->setText((QString::number(bushusum)));//步数写入
            score=100-bushusum/10.0;
            ui->score_label->setText((QString::number(score)));
            }
        }
        break;
    case Qt::Key_Down:
        if(y<2){
            pCompare[x][y]=pCompare[x][y+1];
            pCompare[x][y+1]=8;
            y++;
            moveImage();
            bushusum++;
            if(bushusum>1000)
                return ;
            else
            {
            ui->label_3->setText((QString::number(bushusum)));//步数写入
            score=100-bushusum/10.0;
            ui->score_label->setText((QString::number(score)));
            }
        }
        break;
    case Qt::Key_Right:
        if(x<2){
            pCompare[x][y]=pCompare[x+1][y];
            pCompare[x+1][y]=8;
            x++;
            moveImage();
            bushusum++;
            if(bushusum>1000)
                return ;
            else
            {
            ui->label_3->setText((QString::number(bushusum)));//步数写入
            score=100-bushusum/10.0;
            ui->score_label->setText((QString::number(score)));
            }
        }
        break;
    case Qt::Key_Left:
        if(x>0){
            pCompare[x][y]=pCompare[x-1][y];
            pCompare[x-1][y]=8;
            x--;
            moveImage();
            bushusum++;
            if(bushusum>1000)
                return ;
            else
            {
            ui->label_3->setText((QString::number(bushusum)));//步数写入
            score=100-bushusum/10.0;
            ui->score_label->setText((QString::number(score)));
            }
        break;
        }
    }
}
