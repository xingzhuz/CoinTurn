#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QLabel>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
   // explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int num);

    // 记录所选关卡
    int leavelIndex;
    // 重写绘图事件
    void paintEvent(QPaintEvent *);

    int gameArry[4][4];  // 二维数组维护每个关卡的具体数据
    MyCoin *coinBtn[4][4];

    // 胜利标志
    bool isWin;
    PlayScene *play;

    // 胜利后的下一关按钮和图片封装以及动画
    void winnext_Btn(QPushButton *winButton);
    void win_picture(QLabel *winLabel);
    // 这里使用多态的思想，同时对 QPushButton 和 QLabel 封装
    void win_Animation(QWidget *temp);

signals:
    void chooseBack();

public slots:
};

#endif // PLAYSCENE_H
