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
    explicit Widget(QWidget* parent = nullptr);  // ���캯��
    ~Widget();  // ����������

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;  // ��дeventFilter�������ض��¼���

private:
    QLabel* roleLabel;  // QLabelָ�룬������ʾ��ɫ������
    QMovie* roleAnimation;  // QMovieָ�룬������ʾ������GIF��
    QPushButton* closeBtn;  // QPushButtonָ�룬���ڹرմ��ڵİ�ť��
    QPushButton* switchGifBtn;  // QPushButton�������л�GIFͼƬ��
    QVector<QString> gifPaths;  // QVector�ַ������飬�洢GIF�ļ���·����
    int currentGifIndex;  // ��������¼��ǰ��ʾ��GIF�������е�������

    void initBtn();  // ��Ա���������ڳ�ʼ����ť��
    void changeGIF();  // ��Ա�����������л�����һ��GIF��
    void setupRoleLabel();  // ��Ա��������ʼ����ʾ��ɫ��Label��
    QPushButton* minimizeBtn;  // QPushButtonָ�룬������С�����ڡ�
    QMediaPlayer* musicPlayer;  // QMediaPlayerָ�룬�����������֡�

private slots:
    void togglePlayPause();  // Qt�ۺ���������������/��ͣ���ֵĶ�����
};