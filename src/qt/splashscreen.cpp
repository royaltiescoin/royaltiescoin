#include "splashscreen.h"
#include "clientversion.h"
#include "util.h"

#include <QApplication>

#include <QDesktopWidget>

SplashScreen::SplashScreen(QWidget *parent) :
    QWidget(parent)
{

    QRect rec = QApplication::desktop()->screenGeometry();

    int screenWidth = rec.width();
    int screenHeight = rec.height();

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(screenWidth/2-160,screenHeight/2-170,320,360);


    QPixmap bgPixmap(320,360);

    QLinearGradient bgGradient(QPointF(0, 0), QPointF(screenWidth, 0));
    bgGradient.setColorAt(0, QColor("#FEFEFE"));//#6c3d94"));
    bgGradient.setColorAt(1, QColor("#FEFEFE"));//"#a13469"));
    //#3c3c3b

    QRect rect_linear(0,0,320,360);

    QPainter *painter = new QPainter(&bgPixmap);
    painter->fillRect(rect_linear, bgGradient);

    painter->end();

    bg = new QLabel(this);
    bg->setPixmap(bgPixmap);


    bg->setGeometry(0,0,320,360);

    splashImage = new QLabel(this);
    QPixmap newPixmap;
    if(GetBoolArg("-testnet")) {
        newPixmap.load(":/images/splash_testnet");
    }
    else {
        newPixmap.load(":/images/splash");
    }


    splashImage->setPixmap(newPixmap);
    //splashImage->move(screenWidth/2-160,screenHeight/2-160);


    QFont smallFont; smallFont.setPixelSize(12);

    /*versionLabel = new QLabel(this);
    versionLabel->setStyleSheet("QLabel { color: #3C3C3B; }");
    versionLabel->setFont(smallFont);
    versionLabel->setText(QString::fromStdString(FormatFullVersion()).split("-")[0]);
    versionLabel->setFixedSize(1000,30);
    versionLabel->move(screenWidth/2-108,220);*/


    QFont largeFont; largeFont.setPixelSize(20);

    label = new QLabel(this);
    label->setStyleSheet("QLabel { color: #000000; }");
    label->setFont(largeFont);
    label->setText("...");
    label->setAlignment(Qt::AlignCenter);
    label->setFixedSize(320,30);
    label->move(0,325);

}

SplashScreen::~SplashScreen()
{
}
