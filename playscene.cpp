#include "playscene.h"
#include "mycoin.h"
#include "dataconfig.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <QPropertyAnimation>
#include <QSound>

PlayScene::PlayScene(int num)
{

    this->leavelIndex = num;

    // 初始化游戏场景
    this->setFixedSize(560, 868);
    setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("关卡");

    // 返回按钮
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    // 添加音效
    // 返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav", this);
    // 翻金币按钮音效
    QSound *turnSound = new QSound(":/res/ConFlipSound.wav", this);
    // 胜利音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav", this);


    // 点击返回
    connect(backBtn, &QPushButton::clicked, [=]{
        // 返回音效
        backSound->play();
        this->hide();
        emit this->chooseBack();
    });

    // -----左下角显示当前的关卡数---------------------------------------
    QLabel *label = new QLabel(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str = QString("level: %1").arg(this->leavelIndex);
    // 将字体设置到标签控件中
    label->setFont(font);
    label->setText(str);
    // 设置位置和大小，x, y, width, height 是这四个参数
    label->setGeometry(200, this->height() - 270, 170, 50);


    // 初始化每个关卡的二维数组---------------------------------------------------
    dataConfig config;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            this->gameArry[i][j] = config.mData[this->leavelIndex][i][j];
        }
    }

    // ---胜利----------------------------------------
    // 胜利图片显示
    QLabel* winLabel = new QLabel;
    win_picture(winLabel);

    // 胜利后的下一关图标
    QPushButton* winButton = new QPushButton(this);
    winnext_Btn(winButton);



    // 显示金币背景图案
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            // 绘制背景图片
            QPixmap pix = QPixmap(":/res/BoardNode.png");
            int newWidth = static_cast<int>(pix.width() * 1.5);
            int newHeight = static_cast<int>(pix.height() * 1.5);
            pix = pix.scaled(newWidth, newHeight);

            QLabel *label = new QLabel(this);
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->move(126 + i * 75, 270 + j * 75);

            // 创建金币
            QString str;
            if(gameArry[i][j] == 1) {
                // 显示
                str = ":/res/Coin0001.png";
            }
            else {
               str = ":/res/Coin0008.png";
            }
            MyCoin *coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(129 + i * 75, 275 + j * 75);

            // 给金币属性赋值
            coin->posX = i, coin->posY = j;
            coin->flag = this->gameArry[i][j];  // 1 正面，0 反面

            // 将金币放入金币的二维数组中
            coinBtn[i][j] = coin;

            // 点击金币，进行翻转
            connect(coin, &MyCoin::clicked, [=]{
                // 播放翻金币音效
                turnSound->play();

                // 点击金币后禁用金币点击，周围翻转完成后解开
                // 防止有人点击过快，刚胜利又点击其他的，导致胜利界面出现问题
                for(int i = 0; i < 4; i++)
                    for(int j = 0; j < 4; j++)
                        coinBtn[i][j]->isWin = true;

                coin->changeFlag();
                this->gameArry[i][j] = this->gameArry[i][j] == 0 ? 1 : 0;

                // 翻转周围硬币，延时翻转
                QTimer::singleShot(200, this, [=]{
                    // 右侧
                    if(coin->posX + 1 <= 3) {
                        coinBtn[coin->posX + 1][coin->posY] -> changeFlag();
                        this->gameArry[coin->posX + 1][coin->posY] = this->gameArry[coin->posX + 1][coin->posY] == 0 ? 1 : 0;

                    }
                    // 周围的左侧翻转
                    if(coin->posX -1 >= 0) {
                        coinBtn[coin->posX - 1][coin->posY] -> changeFlag();
                        this->gameArry[coin->posX - 1][coin->posY] = this->gameArry[coin->posX - 1][coin->posY] == 0 ? 1 : 0;
                    }
                    // 上
                    if(coin->posY + 1 <= 3) {
                        coinBtn[coin->posX][coin->posY + 1] -> changeFlag();
                        this->gameArry[coin->posX][coin->posY + 1] = this->gameArry[coin->posX][coin->posY + 1] == 0 ? 1 : 0;
                    }
                    if(coin->posY - 1 >= 0) {
                        coinBtn[coin->posX][coin->posY - 1] -> changeFlag();
                        this->gameArry[coin->posX][coin->posY - 1] = this->gameArry[coin->posX][coin->posY - 1] == 0 ? 1 : 0;
                    }

                    for(int i = 0; i < 4; i++)
                        for(int j = 0; j < 4; j++)
                            coinBtn[i][j]->isWin = false;

                    // 判断是否胜利
                    this->isWin = true;
                    for(int i = 0; i < 4; i++) {
                        for(int j = 0; j < 4; j++) {
                            if(coinBtn[i][j]->flag == false) {
                                this->isWin = false;
                                break;
                            }
                        }
                    }
                    if(this->isWin)
                    {
                        // 添加胜利音效
                        winSound->play();
                        // 金币按钮关闭点击权限
                        for(int i = 0; i < 4; i++)
                            for(int j = 0; j < 4; j++)
                                coinBtn[i][j]->isWin = true;

                        // 胜利动画的封装
                        win_Animation(winLabel);
                        win_Animation(winButton);

                        connect(winButton, &QPushButton::clicked, [=]{
                            play = new PlayScene(num + 1);

                            // 设置下一个场景为当前位置
                            play->setGeometry(this->geometry());
                            play->show();
                            QTimer::singleShot(500, this, [=] {
                                   this->close();
                                   delete this;
                             });

                        });
                    }
                });
            });
        }
    }
}

// 胜利后的按钮封装
void PlayScene::winnext_Btn(QPushButton *winButton)
{
    QPixmap tmpPix;
    // 赢了之后的下一关的按钮
    tmpPix.load(":/res/next.webp");
    tmpPix = tmpPix.scaled(80, 80);
    // 设置按钮的大小和图标
    winButton->setFixedSize(tmpPix.size());
    winButton->setIcon(QIcon(tmpPix));
    winButton->setIconSize(tmpPix.size());
    // 设置按钮的外观，使其无背景和边框
    winButton->setStyleSheet("background-color: rgba(0, 0, 0, 0); border: none;");
    // 设置按钮的位置
    winButton->move((this->width() - tmpPix.width()) * 0.9,  -tmpPix.height());
}

// 胜利后图片的封装
void PlayScene::win_picture(QLabel *winLabel)
{
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - tmpPix.width()) * 0.5 , -tmpPix.height());
}

// 胜利后图片和按钮的动画
void PlayScene::win_Animation(QWidget *temp)
{
    QPropertyAnimation *animation1 = new QPropertyAnimation(temp, "geometry");
    // 设置时间间隔
    animation1->setDuration(1000);
    // 设置开始和结束位置的位置
    animation1->setStartValue(QRect(temp->x(), temp->y(), temp->width(), temp->height()));
    animation1->setEndValue(QRect(temp->x(), temp->y() + 200, temp->width(), temp->height()));

    // 设置曲线
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    animation1->start();
}

void PlayScene::paintEvent(QPaintEvent *)
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
    painter.drawPixmap((560-390)/8, 0, 300, 100,  pix);
}
