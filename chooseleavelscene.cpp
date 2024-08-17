#include "chooseleavelscene.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QSound>

ChooseLeavelScene::ChooseLeavelScene(QWidget *parent) : QMainWindow(parent)
{
    // 配置选择关卡的信息
    this->setFixedSize(560, 868);

    setWindowIcon(QPixmap(":/res/Coin0001.png"));
    setWindowTitle("选择关卡");

    // 返回按钮，这里两个参数，因为有两种效果图
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    // 返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav", this);

    // 点击返回
    connect(backBtn, &QPushButton::clicked, [=]{
        // 播放返回按钮的音效
        backSound->play();
        emit this->ChooseBack();
    });

    // 选择关卡的音效
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav", this);

    // 创建关卡的按钮
    // ChooseLeavelScene 对象被创建时，构造函数运行，for 循环执行一次，生成 20 个关卡按钮和标签，并为每个按钮设置点击事件连接
    // 注意跳转窗口的时候是隐藏，实际还在，调用 show 方法的时候是将隐藏状态变为显示状态
    // 在 delete 具体关卡的对象后，回到当前窗口，由于之前创建在堆区，就是各个关卡按钮，所以一直存在，只需要再次点击，就能再次进入各个关卡
    // 点击的时候创 playscene 对象
    for(int i = 0; i < 20; i++)
    {
        MyPushButton *menBtn = new MyPushButton(":/res/LevelIcon.png");
        menBtn->setParent(this);
        // 这是压缩的一维，100是偏移量
        menBtn->move(100 + i % 4 * 100, 250+ i / 4 * 100);

        // 监听每个按钮的点击事件
        connect(menBtn, &QPushButton::clicked, [=]{
            // 播放音效
            chooseSound->play();

            // 进入关卡
            this->hide();
            // i + 1 表示这是第几关
            play = new PlayScene(i + 1);
           // next = new PlayScene(i + 1, i + 2);
            // 设置下一个场景为当前位置
            play->setGeometry(this->geometry());
            play->show();

            // 监听每个关卡的返回信号
            // 按理说应该放外面更合适，不然每次
            connect(play, &PlayScene::chooseBack, [=]{
                this->setGeometry(play->geometry());

                // 这里直接释放内存，就不用 hide play的关卡了，因为我们定义的局部 new 的变量，直接释放
                this->show();
                delete play;
                play = nullptr;
            });
        });

        // 显示按钮上的各个关卡的数字
        QLabel * label = new QLabel(this);
        label->setFixedSize(menBtn->width(), menBtn->height());
        label->setText(QString::number(1 + i));
        label->move(100 + i % 4 * 100, 250+ i / 4 * 100);

        // 设置字体
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        label->setFont(font);
        // 设置对齐方式
        label->setAlignment(Qt::AlignCenter);
        // 设置让鼠标可以穿透，因为这个 label 盖在鼠标上面了，点击事件无法进行
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}


void ChooseLeavelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    // 画背景大标题图标，复用 pix
    pix.load(":/res/Title.png");
    // 背景图片缩放
    // pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap((560-390)/2, 0, 390, 200,  pix);
}
