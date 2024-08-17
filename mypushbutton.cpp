#include "mypushbutton.h"
#include <QDebug>
#include <QPropertyAnimation>

MyPushButton::MyPushButton(QString normalImg, QString pressImg)
{
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    // 自定义的一个方法，封装了按钮的定义实现
    setButton(this->normalImgPath);
}

void MyPushButton::setButton(QString path)
{
    QPixmap pix;
    bool flag = pix.load(path);
    if(!flag) {
        qDebug() << "图片显示失败！";
        return;
    }

    int newWidth = static_cast<int>(pix.width() * 1.5);
    int newHeight = static_cast<int>(pix.height() * 1.5);
    pix = pix.scaled(newWidth, newHeight);

    // 设置按钮固定大小，使用图片填充
    // 如果不设置并且下述设置了不规则样式，那么将什么都不显示
    setFixedSize(pix.width(), pix.height());

    // 边框去掉，背景也去掉，成为不规则样式
    this->setStyleSheet("QPushButton{border:0px;}");

    // 设置按钮图标
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(), pix.height()));
}

// 往上跳的动画
void MyPushButton::jump_top()
{
    // 创建动画对象
    // 参数 1：给谁做动画，参数 2：什么动画 "geomerty" 矩形框范围动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    // 设置动画的时间间隔
    animation->setDuration(200);

    // 起始位置和结束位置
    animation->setStartValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));

    // 设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

// 往下跳的动画
void MyPushButton::jump_bottom()
{
    // 创建动画对象
    // 1：给谁做动画，2：什么动画 "geomerty" 矩形框范围动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    // 设置动画的时间间隔
    animation->setDuration(200);

    // 起始位置和结束位置
    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    animation->setEndValue(QRect(this->x(), this->y() + 10, this->width(), this->height()));

    // 设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    // 用传入的图片是否为空判断是开始按钮还是返回按钮
    if(this->pressImgPath != "") {
        this->setButton(this->pressImgPath);
    }
    // 让父类执行其他内容
    return QPushButton::mousePressEvent(e);
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    // 用传入的图片是否为空判断是开始按钮还是返回按钮
    if(this->pressImgPath != "") {
        setButton(this->normalImgPath);
    }
    return QPushButton::mouseReleaseEvent(e);
}
