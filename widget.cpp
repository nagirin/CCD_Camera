
#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsDropShadowEffect>
#include <QClipboard>
#include <QTime>
#include <QProcess>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setProperty("canMove",true);
    this->initUi();
    this->initMember();
    pv = new Private();

    pv->cap.open(0);
    pv->cap.m_id = 0;
    connect(&pv->timer, SIGNAL(timeout()), this, SLOT(doCapture()));
    pv->timer.setInterval(0);
    pv->timer.start();
}

Widget::~Widget()
{
    delete ui;
    pv->cap.close();
    delete pv;

}



/**********************************************************初始化函数****************************************************************/


void Widget::initUi()
{
    //初始化窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(16);
    ui->w_bg->setGraphicsEffect(shadow);
    ui->lay_bg->setMargin(12);

    //Logo
    QPixmap logo(":/icons/camera.png");
    ui->lab_logo->setPixmap(logo);

    //itemLogo
    QPixmap item1(":/icons/item_01.png");
    QPixmap item2(":/icons/item_02.png");
    ui->lab_item_01->setPixmap(item1);
    ui->lab_item_02->setPixmap(item2);
}

void Widget::initMember()
{
    this->myMbox = new MyMessageBox();
    connect(myMbox,SIGNAL(btnchicked(int)),this,SLOT(changeMboxReturn(int)));

    //最小化到托盘
    QIcon icon = QIcon(":/icons/camera.png");//设置最小图标
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Camera"); //提示文字
    //添加托盘列表项(还原与退出)
    returnNormal = new QAction(" Show", this);
    returnNormal->setFont(QFont("Arial", 9));
    returnNormal->setObjectName("returnNormal");
    returnNormal->setIcon(QIcon(":/icons/show.png"));
    quitAction = new QAction(" Quit", this);
    quitAction->setFont(QFont("Arial", 9));
    quitAction->setObjectName("quitAction");
    quitAction->setIcon(QIcon(":/icons/out.png"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));           //绑定槽函数退出
    connect(returnNormal, SIGNAL(triggered()), this, SLOT(showNormal()));   //绑定槽函数还原界面

    //创建托盘菜单(必须先创建动作，后添加菜单项，还可以加入菜单项图标美化)
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(returnNormal);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);

    connect(ui->button_Snap, &QPushButton::clicked, this, &Widget::Snap);
    connect(ui->button_Browse, &QPushButton::clicked, this, &Widget::Browse);
    connect(ui->button_Train, &QPushButton::clicked, this, &Widget::Train);
    connect(ui->btn_mine, &QPushButton::clicked, this, &Widget::Switch);
}

/**********************************************************自定义函数****************************************************************/

void Widget::littleShow()
{
    this->hide();//隐藏主窗口
    trayIcon->show();//显示托盘

    //显示到系统提示框的信息
    QString title="Peach";
    QString text="正自动在后台运行";
    trayIcon->showMessage(title,text,QSystemTrayIcon::Information,3000); //此参数为提示时长
}

/**********************************************************控件槽函数****************************************************************/

void Widget::closeEvent(QCloseEvent *event)
{
    myMbox->setIcon("question");
    myMbox->setBodyText("How to leave?");
    myMbox->setButtonText("Little","Logout");
    myMbox->exec();
    if(mbox_return == 0) {
        event->ignore();
    }else if(mbox_return == 1){
        event->ignore();
        littleShow();
    }
}

void Widget::changeMboxReturn(int num)
{
    this->mbox_return = num;
}

void Widget::on_btn_main_item_1_clicked()
{
//    ui->sw_main->setCurrentIndex(0);
}

void Widget::on_btn_main_item_2_clicked()
{
    //ui->sw_main->setCurrentIndex(1);
}

void Widget::on_btn_main_item_3_clicked()
{
    //ui->sw_main->setCurrentIndex(2);
}

void Widget::on_btn_main_item_4_clicked()
{
    //ui->sw_main->setCurrentIndex(3);
}

void Widget::on_btn_logout_clicked()
{
    close();
}

void Widget::on_btn_littleshow_clicked()
{
    showMinimized();
}

void Widget::doCapture()
{
    QImage image = pv->cap.capture();
    int w = image.width();
    int h = image.height();
    if (w > 0 && h > 0) {
        //setFixedSize(w, h);
        ui->widget_4->setFixedSize(w, h);
        ui->widget_4->setImage(image);
    }
}

void Widget::Snap()
{
    QString _SnapPath;
    //step 1 ：选择获取路径
    QString currentPath = QDir::currentPath();

    {
        qDebug() << "currentPath=" << currentPath;

        currentPath += "/";

        std::string m_path = currentPath.toStdString();

        //step 2 :创建Snap保存文件夹
        std::string temp_Path = m_path + "SnapImage/";
        _SnapPath = QString::fromStdString(temp_Path);

        //创建文件夹
        QDir *snapFile = new QDir;
        bool exist = snapFile->exists(_SnapPath);
        if (exist)
        {
            qDebug() << "exist" << _SnapPath;
        }
        else
        {
            //创建文件夹
            bool ok = snapFile->mkdir(_SnapPath);
            //判断是否成功
            if (ok)
            {
                qDebug() << "ok";
            }
            else
            {
                qDebug() << "no";
            }
        }
    }

    QString path =QString ("/%1.png").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));
    //QString path = QFileDialog::getSaveFileName(this, tr("Save as"), QString(), "JPEG files (*.jpg);;PNG files (*.png)");
    if (path.isEmpty()) return;

    QImage image = ui->widget_4->getImage();
    image.save(_SnapPath+ path);
}


void Widget::Browse()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    if (fileName.isEmpty()) return;
    cv::Mat image = cv::imread(fileName.toLatin1().data());
    cv::namedWindow("Image");
    cv::imshow("Image", image);

}


void Widget::Train()
{
    QProcess p(0);
    //p.start("cmd", QStringList()<<"/c"<<" python E:/demo1_alexnet23.3.23/tree3s_main.py");
    p.startDetached("cmd", QStringList()<<"/c"<<" python E:/demo1_alexnet23.3.23/tree3s_main.py");
    p.waitForStarted();
    p.waitForFinished();

}

void Widget::Switch()
{
    if(pv->cap.m_id == 0)
    {
    pv->cap.close();
    pv->cap.open(1);
    pv->cap.m_id = 1;
    }

    else
    {
    pv->cap.close();
    pv->cap.open(0);
    pv->cap.m_id = 0;
    }
}

