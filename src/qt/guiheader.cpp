#include "guiheader.h"

#include <QFormLayout>
#include <QTimer>
#include <qsystemtrayicon.h>

#include "main.h"
#include "bitcoinrpc.h"
#include "util.h"

#include "clientmodel.h"
#include "walletmodel.h"
#include "bitcoinunits.h"

#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"
#include "guiconstants.h"

int screenId = 0;

extern json_spirit::Value GetNetworkHashPS(int lookup, int height);
extern json_spirit::Value ValueFromAmount(int64 amount);
extern int64 GetTotalSupply();


ClickableLabel::ClickableLabel(QWidget * parent ) :
    QLabel(parent)
{
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked();
}

GuiHeader::~GuiHeader(){
}

GuiHeader::GuiHeader(QWidget *parent) :
    QWidget(parent),
    clientModel(0),
    walletModel(0),
    currentBalance(-1),
    currentUnconfirmedBalance(-1),
    currentImmatureBalance(-1)
{

    setStyleSheet("color:#FFFFFF;");

//    QFont largeFont; largeFont.setPixelSize(20);
//    QFont boldFont;
//    boldFont.setBold(true);
//    QFont font;
//    font.setFamily(":/font/montserrat-bold");
//    font.setPixelSize(12);
//    font.setStyleStrategy(QFont::PreferAntialias);
//    font.setBold(true);

//    QFont fontV;
//    fontV.setFamily(":/font/montserrat-bold");
//    fontV.setPixelSize(9);
//    fontV.setStyleStrategy(QFont::PreferAntialias);
//    fontV.setBold(false);

    QFont boldFont; boldFont.setBold(true);
    //QFont font;   font.setFamily(":/font/montserrat-bold");       font.setPixelSize(12);       font.setStyleStrategy(QFont::PreferAntialias);       font.setBold(true);
    //QFont fontV;   fontV.setFamily(":/font/montserrat-bold");       fontV.setPixelSize(9);       fontV.setStyleStrategy(QFont::PreferAntialias);       fontV.setBold(false);

    QFont font;   font.setPixelSize(12);       font.setStyleStrategy(QFont::PreferAntialias);       font.setBold(true);
    QFont fontV;     fontV.setPixelSize(9);       fontV.setStyleStrategy(QFont::PreferAntialias);       fontV.setBold(false);


    QString css1 = "QLabel{font: normal 11px \"Open Sans\";} QToolTip{font: normal 12px \"Open Sans\"; color:#000000; border-style: outset;border-width: 1px;}";
    QString css2 = "QLabel{font: bold 11px \"Open Sans\";} QToolTip{font: normal 12px \"Open Sans\";  color:#000000; border-style: outset;border-width: 1px;}";



    this->setFixedSize(1000,120);

    backGround = new QWidget(this);
    backGround->setFixedSize(1000,120);
    //backGround->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #6c3d94, stop: 1 #a13469)");
    backGround->setStyleSheet("background-color: #0d2846;");

    bottomLine = new QWidget(this);
    bottomLine->setFixedSize(1000,5);
    bottomLine->move(0,115);
    bottomLine->setStyleSheet("background-color: #0f2f51");

    spreadCoinLogo = new ClickableLabel(this);
    spreadCoinLogo->setPixmap( QPixmap(":/icons/royaltycoin_logo_88"));//gui_header_logo"));
    spreadCoinLogo->setContentsMargins(0,0,0,0);
    spreadCoinLogo->move(15,35-10-10);
    spreadCoinLogo->adjustSize();

    royLogoText = new QLabel(this);
    royLogoText->setPixmap(QPixmap(":/images/roy_logo_text"));
    royLogoText->setContentsMargins(0,0,0,0);
    royLogoText->move(119,50-2);
    royLogoText->adjustSize();

    versionLabel = new QLabel(this);
    versionLabel->setFont(fontV);
    versionLabel->move(0,90);
    versionLabel->setFixedWidth(95);
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("color: #000000");
    versionLabel->setVisible(false);
    //versionLabel->setText("v 0.9.15.5");

    /*QWidget *separatorLine = new QWidget(this);
    separatorLine->setFixedSize(2,115);
    separatorLine->move(93,0);
    separatorLine->setStyleSheet("background-color: #3c3c3b");*/

    walletOverview = new QFrame(this);
    walletOverview->setStyleSheet(".QFrame {background-color: #0181ba;}");


    QWidget *walletOverviewBox = new QWidget(walletOverview);
    //walletOverviewBox->setStyleSheet(".QWidget {border: none; background-color: #0181ba;}");

    balanceTitle = new QLabel();
    //balanceTitle->setFont(boldFont);
    balanceTitle->setText("Balance:");
    balanceTitle->setCursor(Qt::IBeamCursor);
    balanceTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);
    balanceTitle->setStyleSheet(css1);

    unconfirmedTitle = new QLabel();
    //unconfirmedTitle->setFont(boldFont);
    unconfirmedTitle->setText("Unconfirmed:");
    unconfirmedTitle->setCursor(Qt::IBeamCursor);
    unconfirmedTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);
    unconfirmedTitle->setStyleSheet(css1);

    immatureTitle = new QLabel();
    //immatureTitle->setFont(boldFont);
    immatureTitle->setText("Immature:");
    immatureTitle->setCursor(Qt::IBeamCursor);
    immatureTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);
    immatureTitle->setStyleSheet(css1);

    labelBalance = new QLabel();
    //labelBalance->setFont(boldFont);
    labelBalance->setText("---");
    labelBalance->setCursor(Qt::IBeamCursor);
    labelBalance->setTextInteractionFlags(Qt::TextSelectableByMouse);
    labelBalance->setStyleSheet(css2);


    labelUnconfirmedBalance = new QLabel();
    //labelUnconfirmedBalance->setFont(boldFont);
    labelUnconfirmedBalance->setText("---");
    labelUnconfirmedBalance->setCursor(Qt::IBeamCursor);
    labelUnconfirmedBalance->setTextInteractionFlags(Qt::TextSelectableByMouse);
    labelUnconfirmedBalance->setStyleSheet(css2);


    labelImmatureBalance = new QLabel();
    //labelImmatureBalance->setFont(boldFont);
    labelImmatureBalance->setText("---");
    labelImmatureBalance->setCursor(Qt::IBeamCursor);
    labelImmatureBalance->setTextInteractionFlags(Qt::TextSelectableByMouse);
    labelImmatureBalance->setStyleSheet(css2);


    walletTitle = new QLabel(walletOverview);
    //walletTitle->setStyleSheet("QLabel { color: #FFFFFF; }");
    walletTitle->setStyleSheet("QLabel { color: #FFFFFF; font: bold 13px \"Open Sans\";}");
    walletTitle->setText("Wallet <font color=#CC0000>(out of sync)</font>");
    walletTitle->setFixedWidth(200);
    walletTitle->move(10,1);

    QFormLayout *formLayout = new QFormLayout();

    formLayout->addRow(balanceTitle, labelBalance);
    formLayout->addRow(unconfirmedTitle, labelUnconfirmedBalance);
    formLayout->addRow(immatureTitle, labelImmatureBalance);


    formLayout->setContentsMargins(10,30,10,10);
    formLayout->setVerticalSpacing(8);
    formLayout->setHorizontalSpacing(10);
    walletOverviewBox->setLayout(formLayout);



    QHBoxLayout  *boxLayout = new QHBoxLayout();
    boxLayout->addWidget(walletOverviewBox);
    boxLayout->setContentsMargins(0,0,0,0);
    walletOverview->setLayout(boxLayout);

    walletOverview->setMinimumWidth(250);
    walletOverview->setMaximumWidth(250);





    recentOverview = new QFrame(this);
    recentOverview->setStyleSheet(".QFrame {background-color: #0181ba;}");

    QWidget *recentOverviewBox = new QWidget(recentOverview);
    //recentOverviewBox->setStyleSheet(".QWidget {border: 1px solid #AFAFAF; background-color: rgba(0,0,0, 15);}");

    QLabel *recentTitle = new QLabel(recentOverview);
    recentTitle->setStyleSheet("QLabel { color: #FFFFFF; font: bold 13px \"Open Sans\";}");
    recentTitle->setText("Recent Events");
    recentTitle->setFixedWidth(150);
    recentTitle->move(10,1);

    for(int a=0;a<3;a++){
        recentLabel[a][0] = new ClickableLabel(recentOverview);
        //recentLabel[a][0]->setFont(boldFont);
        recentLabel[a][1] = new ClickableLabel(recentOverview);
        recentLabel[a][2] = new ClickableLabel(recentOverview);
        recentLabel[a][0]->setStyleSheet(css2);
        recentLabel[a][1]->setStyleSheet(css1);
        recentLabel[a][2]->setStyleSheet(css1);
    }

    for(int b=0;b<3;b++){
        recentOverviewCol[b] = new QWidget(recentOverview);
        recentOverviewCol[b]->setStyleSheet("border: none; background: none;");
        recentColLayout[b] = new QVBoxLayout();

        recentColLayout[b]->addWidget(recentLabel[0][b]);
        recentColLayout[b]->addWidget(recentLabel[1][b]);
        recentColLayout[b]->addWidget(recentLabel[2][b]);

        recentColLayout[b]->setContentsMargins(0,0,0,0);
        recentColLayout[b]->setSpacing(0);
        recentOverviewCol[b]->setLayout(recentColLayout[b]);
    }

    QHBoxLayout  *allRows = new QHBoxLayout();
    allRows->addWidget(recentOverviewCol[0]);
    allRows->addWidget(recentOverviewCol[1]);
    allRows->addWidget(recentOverviewCol[2]);
    allRows->setContentsMargins(10,30-3,10,5);
    allRows->setSpacing(10);
    recentOverviewBox->setLayout(allRows);

    QHBoxLayout  *boxLayout2 = new QHBoxLayout();
    boxLayout2->addWidget(recentOverviewBox);
    boxLayout2->setContentsMargins(0,0,0,0);
    recentOverview->setLayout(boxLayout2);

    recentOverview->setMinimumWidth(250);
    recentOverview->setMaximumWidth(800);




    networkOverview = new QFrame(this);
    networkOverview->setStyleSheet(".QFrame {background-color: #0181ba;}");


    QWidget *networkOverviewBox = new QWidget(networkOverview);
    //networkOverviewBox->setStyleSheet(".QWidget {background-color: rgba(0,0,0, 10);}");

    blockCountTitle = new QLabel("Blockcount:");
    blockCountTitle->setCursor(Qt::IBeamCursor);
    blockCountTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);
    blockCountTitle->setStyleSheet(css1);

    moneySupplyTitle = new QLabel("Moneysupply:");
    moneySupplyTitle->setCursor(Qt::IBeamCursor);
    moneySupplyTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);
    moneySupplyTitle->setStyleSheet(css1);

    hashRateTitle = new QLabel("Hashrate:");
    hashRateTitle->setCursor(Qt::IBeamCursor);
    hashRateTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);
    hashRateTitle->setStyleSheet(css1);

    blockCount = new QLabel();
    blockCount->setFont(boldFont);
    blockCount->setText("---");
    blockCount->setCursor(Qt::IBeamCursor);
    blockCount->setTextInteractionFlags(Qt::TextSelectableByMouse);
    blockCount->setStyleSheet(css2);

    moneySupply = new QLabel();
    moneySupply->setFont(boldFont);
    moneySupply->setText("---");
    moneySupply->setCursor(Qt::IBeamCursor);
    moneySupply->setTextInteractionFlags(Qt::TextSelectableByMouse);
    moneySupply->setStyleSheet(css2);

    hashRate = new QLabel();
    hashRate->setFont(boldFont);
    hashRate->setText("---");
    hashRate->setCursor(Qt::IBeamCursor);
    hashRate->setTextInteractionFlags(Qt::TextSelectableByMouse);
    hashRate->setStyleSheet(css2);


    networkTitle = new QLabel(networkOverview);
    networkTitle->setStyleSheet("QLabel { color: #FFFFFF; font: bold 13px \"Open Sans\";}");
    networkTitle->setText("Network <font color=#CC0000>(out of sync)</font>");
    networkTitle->setFixedWidth(200);
    networkTitle->move(10,1);

    QFormLayout *formLayout3 = new QFormLayout();

    formLayout3->addRow(blockCountTitle, blockCount);
    formLayout3->addRow(moneySupplyTitle, moneySupply);
    formLayout3->addRow(hashRateTitle, hashRate);

    formLayout3->setContentsMargins(10,30,10,10);
    formLayout3->setVerticalSpacing(8);
    formLayout3->setHorizontalSpacing(10);
    networkOverviewBox->setLayout(formLayout3);


    QHBoxLayout  *boxLayout3 = new QHBoxLayout();
    boxLayout3->addWidget(networkOverviewBox);
    boxLayout3->setContentsMargins(0,0,0,0);
    networkOverview->setLayout(boxLayout3);

    networkOverview->setMinimumWidth(200);
    networkOverview->setMaximumWidth(250);


    QHBoxLayout  *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(walletOverview);
    mainLayout->addWidget(recentOverview);
    mainLayout->addWidget(networkOverview);//,0,Qt::AlignRight);
    mainLayout->setContentsMargins(121,10,10,15);
    mainLayout->setSpacing(10);
    mainLayout->setAlignment(Qt::AlignRight);
    //mainLayout->setAlignment(Qt::AlignLeft);
    setLayout(mainLayout);

    for(int a=0;a<3;a++){
        for(int b=0;b<3;b++){
            mapper[a][b] = new QSignalMapper( this );
            mapper[a][b]->setMapping(recentLabel[a][b],a);
            connect( recentLabel[a][b], SIGNAL(clicked()), mapper[a][b], SLOT(map()) );
            connect(mapper[a][b], SIGNAL(mapped(int)), SLOT(handleTransactionClicked(int)));
        }
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDisplayUnit()));
    timer->start(500);
}

void GuiHeader::resize(int width)
{
    this->setFixedWidth(width);
    backGround->setFixedWidth(width);
    bottomLine->setFixedWidth(width);
}


void GuiHeader::setBalance(qint64 balance, qint64 unconfirmedBalance, qint64 immatureBalance)
{
    currentBalance = balance;
    currentUnconfirmedBalance = unconfirmedBalance;
    currentImmatureBalance = immatureBalance;

    labelBalance->setText(BitcoinUnits::formatWithUnit(unit, balance));
    labelUnconfirmedBalance->setText(BitcoinUnits::formatWithUnit(unit, unconfirmedBalance));
    labelImmatureBalance->setText(BitcoinUnits::formatWithUnit(unit, immatureBalance));
}

void GuiHeader::setClientModel(ClientModel *model)
{
    this->clientModel = model;

    if(model)
    {
        versionLabel->setText(model->formatFullVersion().split("-")[0]);
    }
}

void GuiHeader::setWalletModel(WalletModel *model)
{
    this->walletModel = model;

    if(model && model->getOptionsModel())
    {
        filter = new TransactionFilterProxy();
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(3);//NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->sort(TransactionTableModel::Status, Qt::DescendingOrder);

        // Keep up to date with wallets
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance());
        connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));
        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));

    }

    // update the display unit, to not use the default ("BTC")
    updateDisplayUnit();
}

void GuiHeader::updateDisplayUnit()
{
    if(walletModel && walletModel->getOptionsModel())
    {
        unit = walletModel->getOptionsModel()->getDisplayUnit();
        if(currentBalance != -1)
            setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance);

        QDateTime now = QDateTime::currentDateTime();

        for(int i=0; i<filter->rowCount();i++){

            QString amountText = BitcoinUnits::formatWithUnit(unit, filter->data(filter->index(i, 0), TransactionTableModel::AmountRole).toLongLong());
            bool confirmed = filter->data(filter->index(i, 0), TransactionTableModel::ConfirmedRole).toBool();
            if(!confirmed)
            {
                amountText = QString("[") + amountText + QString("]");
            }

            recentLabel[i][0]->setText(amountText);
            if(filter->data(filter->index(i, 0), TransactionTableModel::AmountRole).toLongLong() < 0){
                recentLabel[i][1]->setText("Sent");
            } else{
               recentLabel[i][1]->setText("Received");
            }

            QString agoString = "";
            qint64 ago = filter->data(filter->index(i, 0), TransactionTableModel::DateRole).toDateTime().secsTo(now);
            if(ago <60){
                agoString = QString::number(ago)+" sec ago";
            }
            if(ago >=60 && ago <3600){
                agoString = QString::number(ago/60)+" min ago";
            }
            if(ago >=3600 && ago <86400){
                agoString = QString::number(ago/3600)+" h ago";
            }
            if(ago >=86400 && ago <31536000){
                agoString = QString::number(ago/86400)+" d ago";
            }
            if(ago >=31536000){
                agoString = QString::number(ago/86400)+" y ago";
            }

            recentLabel[i][2]->setText(agoString);

            recentLabel[i][0]->setToolTip(filter->data(filter->index(i, 0), TransactionTableModel::ToAddress).toString());
            recentLabel[i][1]->setToolTip(filter->data(filter->index(i, 0), TransactionTableModel::ToAddress).toString());
            recentLabel[i][2]->setToolTip(filter->data(filter->index(i, 0), TransactionTableModel::ToAddress).toString());
        }
    }
}

void GuiHeader::showOutOfSyncWarning(bool fShow)
{
    if(fShow){
        walletTitle->setText("Wallet <font color=#CC0000>(out of sync)</font>");
        networkTitle->setText("Network <font color=#CC0000>(out of sync)</font>");
    } else{
        walletTitle->setText("Wallet");
        networkTitle->setText("Network");
    }
}

static QString formatHashrate(int64 n)
{
    if (n == 0)
        return "0 H/s";

    int i = (int)floor(log(n)/log(1000));
    float v = n*pow(1000.0f, -i);

    QString prefix = "";
    if (i >= 1 && i < 9)
        prefix = " kMGTPEZY"[i];

    return QString("%1 %2H/s").arg(v, 0, 'f', 2).arg(prefix);
}

void GuiHeader::updateNetworkOverview()
{
    blockCount->setText(QString::number(nBestHeight));

    int totalMoney = (double)GetTotalSupply()/(double)COIN;
    moneySupply->setText(QString::number( totalMoney)+" ROY");

    int64 NetworkHashrate = GetNetworkHashPS(120, -1).get_int64();
    hashRate->setText(formatHashrate(NetworkHashrate));

}

void GuiHeader::handleTransactionClicked(int indexa)
{
    if(filter->rowCount() > indexa){
        QModelIndex id = filter->index(indexa,3, QModelIndex());
        emit transactionClicked2(filter->mapToSource(id));
    }
}
