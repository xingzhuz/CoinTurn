#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyPushButton(QObject *parent = nullptr);

    // 构造函数 参数一 正常显示的图片路径，参数二 按下后显示的图片路径
    // 这里第二个参数设置默认参数是因为为了封装多个按钮，有些按钮只有一种显示，有些正常显示和按下显示不同
    // 这样传参就行，方便一起封装
    MyPushButton(QString normalImg, QString pressImg = "");

    // 成员属性，保存路径
    QString normalImgPath;
    QString pressImgPath;

    // 设置各个按钮
    void setButton(QString path);

    // 弹跳特效
    void jump_top();     // 向上弹跳
    void jump_bottom();  // 向下弹跳

    // 重写按钮的按下与释放的事件
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
signals:

public slots:
};

#endif // MYPUSHBUTTON_H
