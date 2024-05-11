// widget.h
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMovie>
#include <QVector>
#include <QMediaPlayer>
#include <QAudioOutput>



class Widget : public QWidget  
{
    Q_OBJECT 

public:
    explicit Widget(QWidget* parent = nullptr);  // 构造函数
    ~Widget();  // 析构函数。

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;  // 重写eventFilter来处理特定事件。

private:
    QLabel* roleLabel;  // QLabel指针，用于显示角色动画。
    QMovie* roleAnimation;  // QMovie指针，用于显示动画如GIF。
    QPushButton* closeBtn;  // QPushButton指针，用于关闭窗口的按钮。
    QPushButton* switchGifBtn;  // QPushButton，用于切换GIF图片。
    QVector<QString> gifPaths;  // QVector字符串数组，存储GIF文件的路径。
    int currentGifIndex;  // 整数，记录当前显示的GIF在数组中的索引。

    void initBtn();  // 成员函数，用于初始化按钮。
    void changeGIF();  // 成员函数，用于切换到下一个GIF。
    void setupRoleLabel();  // 成员函数，初始化显示角色的Label。
    QPushButton* minimizeBtn;  // QPushButton指针，用于最小化窗口。
    QMediaPlayer* musicPlayer;  // QMediaPlayer指针，用来播放音乐。

private slots:
    void togglePlayPause();  // Qt槽函数，用来处理播放/暂停音乐的动作。
};