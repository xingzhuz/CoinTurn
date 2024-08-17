#ifndef CHOOSELEAVELSCENE_H
#define CHOOSELEAVELSCENE_H

#include <QMainWindow>
#include "playscene.h"

class ChooseLeavelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLeavelScene(QWidget *parent = nullptr);

    // 重写绘图事件
    void  paintEvent(QPaintEvent *);

    // 游戏场景
    PlayScene *play = nullptr;

signals:
    // 自定义信号
    void ChooseBack();

public slots:
};

#endif // CHOOSELEAVELSCENE_H
