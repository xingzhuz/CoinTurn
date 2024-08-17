#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QSound>   // 多媒体模块下的头文件，要在 pro 文件中 Qt += multimedia


MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    // 配置主场景
    // 设置固定大小
    setFixedSize(560, 868);
    // 设置窗口图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    // 设置标题
    setWindowTitle("翻金币");



    // 准备开始按钮的音效
    QSound *startSound = new QSound(":/res/TapButtonSound.wav", this);

    // 开始按钮
    MyPushButton *startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move((this->width() - startBtn->width())*0.5, this->height()*0.5);

    // 退出游戏按钮实现
    exitGame();


    // 实例化选择关卡对象
    chooseScene = new ChooseLeavelScene;

    // 监听选择关卡的返回按钮信号
    connect(chooseScene, &ChooseLeavelScene::ChooseBack, [=]{

        this->setGeometry(chooseScene->geometry());

        // 显示主窗口
       chooseScene->hide();
       this->show();
    });

    // 点击开始按钮后
    connect(startBtn, &QPushButton::clicked, [=]{
        // 播放开始按钮的音效资源
        startSound->play();
        startBtn->jump_bottom();
        startBtn->jump_top();

        // 延时进入到选择关卡的场景中
        // 0.5 秒让当前窗口执行 lambda 表达式内容
        QTimer::singleShot(500, this, [=]{
            // 设置一下下一个进入的场景为当前位置，因为窗口可以随意放在屏幕任意位置
            // 这里是自适应，就不用每次都在中心
            chooseScene->setGeometry(this->geometry());

            this->hide();
            chooseScene->show();
        });
    });
}


// 由于使用了 QPushButton 的图标功能来显示图片
// 通过设置按钮的图标，可以利用 Qt 提供的现有控件功能，而不必手动绘制。
void MainScene::exitGame()
{
    // 退出按钮
    QPushButton *exitButton = new QPushButton(this);
    // 加载图片并将其转换为红色
    QPixmap pix(":/res/exit.png");
    QImage img = pix.toImage();
    for (int y = 0; y < img.height(); ++y) {
       for (int x = 0; x < img.width(); ++x) {
           QColor color = img.pixelColor(x, y);
           color.setRed(255);
           color.setGreen(0);
           color.setBlue(0);
           img.setPixelColor(x, y, color);
       }
    }
    QPixmap redPix = QPixmap::fromImage(img);
    QPixmap scaledPix = redPix.scaled(50, 50);

    // 设置按钮的图标为修改后的图片
    exitButton->setIcon(QIcon(scaledPix));
    exitButton->setIconSize(scaledPix.size());

    // 设置按钮外观，使其无背景和边框
    exitButton->setStyleSheet("background-color: rgba(0, 0, 0, 0); border: none;");

    // 连接按钮的点击信号到关闭窗口的槽函数
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);

    // 设置按钮的大小和位置
    exitButton->setFixedSize(50, 50);
    exitButton->move(250, 620);
}

// 画背景图
void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");

    // 第 3、4个参数是将图片拉伸到和当前窗口一样的宽和高
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    // 画背景图标，复用 pix
    pix.load(":/res/Title.png");
    // 背景图片缩放
    // pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap((560-390)/2, 0, 390, 200,  pix);
}

MainScene::~MainScene()
{
    delete ui;
}

