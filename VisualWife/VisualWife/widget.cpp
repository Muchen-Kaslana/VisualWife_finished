// widget.cpp
//��Ҫ��ͷ�ļ���Qtģ�鱻�������������ں�����ඨ��͹���ʵ�֡�
#include "widget.h"
#include <QMovie>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QFileDialog>
#include <QPushButton>


//���幹�캯��
Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , roleLabel(new QLabel(this))
{
    // ȥ�����ڵı߿򣬺��ñ���͸��
    setWindowFlags(Qt::WindowType::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // ʹ��gif����
    roleAnimation = new QMovie(":/new/prefix1/resource/desktopRole/rotate.gif");
    roleLabel->setMovie(roleAnimation);
    roleAnimation->start();
    roleLabel->resize(200, 200);  // ����ʵ�� GIF �ߴ����
    roleAnimation = new QMovie(":/new/prefix1/resource/desktopRole/rotate.gif");
    roleLabel->setMovie(roleAnimation);
    roleAnimation->start();
    setupRoleLabel();  // ʹ���µ����ú���



    // ��ʼ�� GIF ·���б�
    gifPaths = {
        ":/new/prefix1/resource/desktopRole/fuxuan_matting.gif",
        ":/new/prefix1/resource/desktopRole/bailu_matting.gif",
        ":/new/prefix1/resource/desktopRole/xinhai.gif",
        ":/new/prefix1/resource/desktopRole/fufu_matting.gif",
        ":/new/prefix1/resource/desktopRole/huangnv.gif",
        ":/new/prefix1/resource/desktopRole/nilu_matting.gif",
        ":/new/prefix1/resource/desktopRole/rotate.gif",


    };

    // ������������Ӱ,����ǿ�����
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(QColor(230, 231, 232, 220));
    effect->setBlurRadius(5);
    this->setGraphicsEffect(effect);
    this->installEventFilter(this);


    //������ť
    initBtn();


    // ����ý�岥��������
    musicPlayer = new QMediaPlayer(this);
    // ������Ƶ�������
    QAudioOutput* audioOutput = new QAudioOutput(this);
    // �������� (Qt 6 ��QAudioOutput��������float���͵ģ���Χ��0.0��1.0)
    audioOutput->setVolume(1.0); // �������ǽ���ʼ��������Ϊ���
    // ����Ƶ���������ý�岥����
    musicPlayer->setAudioOutput(audioOutput);
    // �������ֵ�Դ·��������ѡ�������ʹһ�������ڰ��ˡ�
    musicPlayer->setSource(QUrl("qrc:/new/prefix1/resource/music.mp3"));
    // ��ʼ����
    musicPlayer->play();
}

//��������
Widget::~Widget()
{
    roleAnimation->stop();
    delete roleAnimation;
}
//��ʼ���˴��ڵĿ��ư�ť������������Ӧ���¼����������ۺ�����
void Widget::initBtn()
{   //�رհ�ť
    //����ͼƬ��ť����ť�����������������������ͣʱΪ��ɫ�����������ʱΪ��ɫ
    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(200, 200, 32, 32);
    closeBtn->setObjectName("closeBtn");
    closeBtn->setStyleSheet("QPushButton{background-image:url(:/new/prefix1/resource/button/quit.png);}"
        "QPushButton:hover{background-color: darkblue; background-image: url(:/new/prefix1/resource/button/quit.png);}"
        "QPushButton:pressed{background-color: red; background-image: url(:/new/prefix1/resource/button/quit.png);}");
    //ͼƬ�л���ť
    switchGifBtn = new QPushButton(this);
    switchGifBtn->setGeometry(200, 150, 32, 32);
    switchGifBtn->setObjectName("switchGifBtn");
    switchGifBtn->setStyleSheet("QPushButton{background-image:url(:/new/prefix1/resource/desktopRole/cut.png);}"
        "QPushButton:hover{background-color: darkblue; background-image: url(:/new/prefix1/resource/desktopRole/cut.png);}"
        "QPushButton:pressed{background-color: red; background-image: url(:/new/prefix1/resource/desktopRole/cut.png);}");
    //��С����ť
    minimizeBtn = new QPushButton(this);
    minimizeBtn->setGeometry(200, 100, 32, 32); 
    minimizeBtn->setObjectName("minimizeBtn");
    minimizeBtn->setStyleSheet("QPushButton{background-image:url(:/new/prefix1/resource/button/sub.png);}"
        "QPushButton:hover{background-color: darkblue; background-image: url(:/new/prefix1/resource/button/sub.png);}"
        "QPushButton:pressed{background-color: red; background-image: url(:/new/prefix1/resource/button/sub.png);}");
    //���ֲ��ź���ͣ��ť
    QPushButton* playPauseBtn = new QPushButton(this);
    playPauseBtn->setGeometry(200, 250, 32, 32); 
    playPauseBtn->setObjectName("playPauseBtn"); 
    playPauseBtn->setStyleSheet(
        "QPushButton{background-image:url(:/new/prefix1/resource/button/music_player.png);}" 
        "QPushButton:hover{background-color: darkblue; background-image: url(:/new/prefix1/resource/button/music_player.png);}"
        "QPushButton:pressed{background-color: red; background-image: url(:/new/prefix1/resource/button/music_player.png);}" 
    );

    //���ӵ��ۺ���
    connect(minimizeBtn, &QPushButton::pressed, this, &Widget::showMinimized);
    connect(closeBtn, &QPushButton::pressed, this, &Widget::close);
    connect(switchGifBtn, &QPushButton::pressed, this, &Widget::changeGIF);
    connect(playPauseBtn, &QPushButton::clicked, this, &Widget::togglePlayPause);
}
//�л�gif��������
void Widget::changeGIF()
{
    currentGifIndex = (currentGifIndex + 1) % gifPaths.size();
    roleAnimation->stop();
    delete roleAnimation;

    roleAnimation = new QMovie(gifPaths[currentGifIndex]);
    setupRoleLabel(); // �ٴε�����Ӧ��ͨ������
    roleAnimation->start();
}

//�¼�������
bool Widget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        static QPoint dragPos;
        if (event->type() == QEvent::MouseButtonPress) {
            // Using globalPosition() and converting the result to QPoint
            dragPos = mouseEvent->globalPosition().toPoint() - frameGeometry().topLeft();
            return true;  // ��ʾ�Ѵ����¼�
        }
        else if (event->type() == QEvent::MouseMove && mouseEvent->buttons() & Qt::LeftButton) {
            // Similarly, use globalPosition().toPoint() for conversion
            move(mouseEvent->globalPosition().toPoint() - dragPos);
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}




//ȷ��ÿһ��gif������ͬ�ߴ�
void Widget::setupRoleLabel()
{
    roleLabel->setMovie(roleAnimation);
    roleLabel->setScaledContents(true);  // ȷ��GIF����label��С����
    roleLabel->resize(200, 200);         // GIF�ߴ����
    // �����ޱ߿�ͱ���͸����ȥ���߿����ñ���͸��
    roleLabel->setStyleSheet("background: transparent; border: none;");
}
//ʵ��������ͣ���ŵķ�������
void Widget::togglePlayPause()
{
    // ������ֲ�������״̬
    if (musicPlayer->playbackState() == QMediaPlayer::PlayingState) {
        musicPlayer->pause(); // ������ڲ��ţ���ô��ͣ����
    }
    else {
        musicPlayer->play(); // ����ʼ����
    }
}