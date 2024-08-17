#include "mycoin.h"
#include <QDebug>

//MyCoin::MyCoin(QWidget *parent) : QPushButton(parent)
//{

//}


MyCoin::MyCoin(QString btnImg)
{
    QPixmap pix;
    bool flag = pix.load(btnImg);
    if(!flag) {
        QString str = QString("图片 %1 加载失败").arg(btnImg);
        qDebug()<< str;
        return;
    }
    // 自定义封装的设置按钮方法
    setButton(pix);

    // 初始化定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    // 监听正面翻反面信号，并且延时多种状态翻转金币
    connect(timer1, &QTimer::timeout, [=]{
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->minn++);
        pix.load(str);

        setButton(pix);

        // 判断如果翻完了，将 minn 重置为 1
        // 翻转状态恢复，为下次翻转准备
        if(this->minn > this->maxn) {
            this->minn = 1;
            timer1->stop();
            isAnimation = false;
        }
    });

    // 监听反面翻正面信号，并且延时多种状态翻转金币
    connect(timer2, &QTimer::timeout, [=]{
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->maxn--);
        pix.load(str);

        setButton(pix);

        // 判断如果翻完了，将 maxn 重置为 8，这是为了后续还能继续玩
        // 翻转状态恢复，为下次翻转准备
        if(this->maxn < this->minn) {
            this->maxn = 8;
            timer2->stop();
            isAnimation = false;
        }
    });
}

void MyCoin::setButton(QPixmap &pix)
{
    // 调整图像大小
    int newWidth = static_cast<int>(pix.width() * 1.5);
    int newHeight = static_cast<int>(pix.height() * 1.5);
    pix = pix.scaled(newWidth, newHeight);

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}

// 改变正反面标志的方法
void MyCoin::changeFlag()
{
    // 如果是正面
    if(this->flag) {
        this->timer1->start(30);
        isAnimation = true;
        this->flag = false;
    }
    else {
        this->timer2->start(30);
        isAnimation = true;
        this->flag = true;
    }
}

void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation || this->isWin) {
        return;
    }
    else {
        QPushButton::mousePressEvent(e);
    }
}
