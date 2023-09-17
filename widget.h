#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//最小化相关
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
//自定义弹窗
#include "mymessagebox.h"
#include "ImageWidget.h"

#include <stdint.h>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QClipboard>
#include <opencv2/opencv.hpp>
class ImageWidget;

namespace Ui {
class Widget;
}


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void closeEvent(QCloseEvent *event);

    void changeMboxReturn(int num);

    void on_btn_main_item_1_clicked();

    void on_btn_main_item_2_clicked();

    void on_btn_main_item_3_clicked();

    void on_btn_main_item_4_clicked();

    void on_btn_logout_clicked();

    void on_btn_littleshow_clicked();

    void doCapture();

    void Snap();

    void Browse();

    void Train();
    void Switch();
private:
    Ui::Widget *ui;
    struct Private;
    Private *pv;

    MyMessageBox *myMbox;
    //自定义窗口通信参数
    int mbox_return;

    //最小化到托盘
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    //窗口管理动作
    QAction *returnNormal;
    QAction *quitAction;

    void initUi();          //Ui界面初始化函数
    void initMember();      //成员变量初始化函数
    void littleShow();      //最小化显示函数
};

class Capture1 {
private:
    cv::VideoCapture cap;
    cv::VideoWriter *video;

public:
    int m_id = 0;
    void open(int index)
    {
        close();
        cap.open(index,cv::CAP_DSHOW);
    }
    void close()
    {
        cap.release();
    }
    bool isOpened() const
    {
        return cap.isOpened();
    }
    QImage capture()
    {
        if (isOpened()) {
            cv::Mat frame;
            cap.read(frame);
            if (frame.dims == 2 && frame.channels() == 3) {
                int w = frame.cols;
                int h = frame.rows;
                QImage image(w, h, QImage::Format_RGB32);
                for (int i = 0; i < h; i++) {
                    uint8_t const *src = frame.ptr(i);
                    QRgb *dst = (QRgb *)image.scanLine(i);
                    for (int j = 0; j < w; j++) {
                        dst[j] = qRgb(src[2], src[1], src[0]);
                        src += 3;
                    }
                }
                return image;
            }
            //video->write(frame);
        }
        return QImage();
    }
};

struct Widget::Private {
    Capture1 cap;

    QTimer timer;
};

#endif // WIDGET_H
