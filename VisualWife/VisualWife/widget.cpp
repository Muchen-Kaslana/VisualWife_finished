// widget.cpp
//需要的头文件和Qt模块被包含进来，用于后面的类定义和功能实现。
#include "widget.h"
#include <QMovie>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QFileDialog>
#include <QPushButton>


//定义构造函数
Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , roleLabel(new QLabel(this))
{
    // 去掉窗口的边框，和让背景透明
    setWindowFlags(Qt::WindowType::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 使用gif动画
    roleAnimation = new QMovie(":/new/prefix1/resource/desktopRole/rotate.gif");
    roleLabel->setMovie(roleAnimation);
    roleAnimation->start();
    roleLabel->resize(200, 200);  // 根据实际 GIF 尺寸调整
    roleAnimation = new QMovie(":/new/prefix1/resource/desktopRole/rotate.gif");
    roleLabel->setMovie(roleAnimation);
    roleAnimation->start();
    setupRoleLabel();  // 使用新的设置函数



    // 初始化 GIF 路径列表
    gifPaths = {
        ":/new/prefix1/resource/desktopRole/fuxuan_matting.gif",
        ":/new/prefix1/resource/desktopRole/bailu_matting.gif",
        ":/new/prefix1/resource/desktopRole/xinhai.gif",
        ":/new/prefix1/resource/desktopRole/fufu_matting.gif",
        ":/new/prefix1/resource/desktopRole/huangnv.gif",
        ":/new/prefix1/resource/desktopRole/nilu_matting.gif",
        ":/new/prefix1/resource/desktopRole/rotate.gif",


    };

    // 给窗口设置阴影,以增强立体感
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(QColor(230, 231, 232, 220));
    effect->setBlurRadius(5);
    this->setGraphicsEffect(effect);
    this->installEventFilter(this);


    //声明按钮
    initBtn();


    // 创建媒体播放器对象
    musicPlayer = new QMediaPlayer(this);
    // 创建音频输出对象
    QAudioOutput* audioOutput = new QAudioOutput(this);
    // 设置音量 (Qt 6 中QAudioOutput的音量是float类型的，范围是0.0到1.0)
    audioOutput->setVolume(1.0); // 这里我们将初始音量设置为最大
    // 将音频输出关联到媒体播放器
    musicPlayer->setAudioOutput(audioOutput);
    // 设置音乐的源路径，我们选择歌曲《使一颗心免于哀伤》
    musicPlayer->setSource(QUrl("qrc:/new/prefix1/resource/music.mp3"));
    // 开始播放
    musicPlayer->play();
}

//析构函数
Widget::~Widget()
{
    roleAnimation->stop();
    delete roleAnimation;
}
//初始化了窗口的控制按钮，并连接了相应的事件处理函数（槽函数）
void Widget::initBtn()
{   //关闭按钮
    //设置图片按钮，按钮对鼠标操作做出反馈，鼠标悬停时为蓝色，鼠标左键点击时为红色
    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(200, 200, 32, 32);
    closeBtn->setObjectName("closeBtn");
    closeBtn->setStyleSheet("QPushButton{background-image:url(:/new/prefix1/resource/button/quit.png);}"
        "QPushButton:hover{background-color: darkblue; background-image: url(:/new/prefix1/resource/button/quit.png);}"
        "QPushButton:pressed{background-color: red; background-image: url(:/new/prefix1/resource/button/quit.png);}");
    //图片切换按钮
    switchGifBtn = new QPushButton(this);
    switchGifBtn->setGeometry(200, 150, 32, 32);
    switchGifBtn->setObjectName("switchGifBtn");
    switchGifBtn->setStyleSheet("QPushButton{background-image:url(:/new/prefix1/resource/desktopRole/cut.png);}"
        "QPushButton:hover{background-color: darkblue; background-image: url(:/new/prefix1/resource/desktopRole/cut.png);}"
        "QPushButton:pressed{background-color: red; background-image: url(:/new/prefix1/resource/desktopRole/cut.png);}");
    //最小化按钮
    minimizeBtn = new QPushButton(this);
    minimizeBtn->setGeometry(200, 100, 32, 32); 
    minimizeBtn->setObjectName("minimizeBtn");
    minimizeBtn->setStyleSheet("QPushButton{background-image:url(:/new/prefix1/resource/button/sub.png);}"
        "QPushButton:hover{background-color: darkblue; background-image: url(:/new/prefix1/resource/button/sub.png);}"
        "QPushButton:pressed{background-color: red; background-image: url(:/new/prefix1/resource/button/sub.png);}");
    //音乐播放和暂停按钮
    QPushButton* playPauseBtn = new QPushButton(this);
    playPauseBtn->setGeometry(200, 250, 32, 32); 
    playPauseBtn->setObjectName("playPauseBtn"); 
    playPauseBtn->setStyleSheet(
        "QPushButton{background-image:url(:/new/prefix1/resource/button/music_player.png);}" 
        "QPushButton:hover{background-color: darkblue; background-image: url(:/new/prefix1/resource/button/music_player.png);}"
        "QPushButton:pressed{background-color: red; background-image: url(:/new/prefix1/resource/button/music_player.png);}" 
    );

    //连接到槽函数
    connect(minimizeBtn, &QPushButton::pressed, this, &Widget::showMinimized);
    connect(closeBtn, &QPushButton::pressed, this, &Widget::close);
    connect(switchGifBtn, &QPushButton::pressed, this, &Widget::changeGIF);
    connect(playPauseBtn, &QPushButton::clicked, this, &Widget::togglePlayPause);
}
//切换gif方法定义
void Widget::changeGIF()
{
    currentGifIndex = (currentGifIndex + 1) % gifPaths.size();
    roleAnimation->stop();
    delete roleAnimation;

    roleAnimation = new QMovie(gifPaths[currentGifIndex]);
    setupRoleLabel(); // 再次调用以应用通用设置
    roleAnimation->start();
}

//事件过滤器
bool Widget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        static QPoint dragPos;
        if (event->type() == QEvent::MouseButtonPress) {
            // Using globalPosition() and converting the result to QPoint
            dragPos = mouseEvent->globalPosition().toPoint() - frameGeometry().topLeft();
            return true;  // 表示已处理事件
        }
        else if (event->type() == QEvent::MouseMove && mouseEvent->buttons() & Qt::LeftButton) {
            // Similarly, use globalPosition().toPoint() for conversion
            move(mouseEvent->globalPosition().toPoint() - dragPos);
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}




//确保每一张gif都是相同尺寸
void Widget::setupRoleLabel()
{
    roleLabel->setMovie(roleAnimation);
    roleLabel->setScaledContents(true);  // 确保GIF根据label大小缩放
    roleLabel->resize(200, 200);         // GIF尺寸调整
    // 设置无边框和背景透明，去掉边框并设置背景透明
    roleLabel->setStyleSheet("background: transparent; border: none;");
}
//实现音乐暂停播放的方法定义
void Widget::togglePlayPause()
{
    // 检查音乐播放器的状态
    if (musicPlayer->playbackState() == QMediaPlayer::PlayingState) {
        musicPlayer->pause(); // 如果正在播放，那么暂停音乐
    }
    else {
        musicPlayer->play(); // 否则开始播放
    }
}