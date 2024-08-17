#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyCoin(QWidget *parent = nullptr);

    // 参数代表传入的是金币路径还是银币路径
    MyCoin(QString btnImg);

    // 金币属性
    int posX;   // X 坐标
    int posY;   // Y 坐标
    bool flag;  // 正反面

    // 改变标志的方法
    void changeFlag();
    QTimer *timer1;    // 正面翻反面定时器
    QTimer *timer2;    // 反面翻正面定时器
    int minn = 1, maxn = 8;   // 资源图片数量的最大值和最小值，标号使用，就是 8 种状态

    // 执行动画的标志
    bool isAnimation = false;

    // 重写按下和释放状态
    void mousePressEvent(QMouseEvent *e);

    bool isWin = false;

    // 设置金币和银币的按钮
    void setButton(QPixmap &pix);

signals:

public slots:
};

#endif // MYCOIN_H
