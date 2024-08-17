#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include "chooseleavelscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainScene; }
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();

    // 重写 paintEvent 事件
    void paintEvent(QPaintEvent *);

    // 选择场景
    ChooseLeavelScene *chooseScene = nullptr;

    // 退出游戏按钮
    void exitGame();


private:
    Ui::MainScene *ui;
};
#endif // MAINSCENE_H
