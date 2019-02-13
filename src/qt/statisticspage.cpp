#include "statisticspage.h"
#include "ui_statisticspage.h"
#include "main.h"
#include "wallet.h"
#include "base58.h"
#include "clientmodel.h"
#include "foxcoinrpc.h"
#include "foxcoinfunction.h"
#include <sstream>
#include <string>

using namespace json_spirit;

StatisticsPage::StatisticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsPage)
{
    ui->setupUi(this);
    
    setFixedSize(400, 420);
    
    connect(ui->startButton, SIGNAL(pressed()), this, SLOT(updateStatistics()));
    connect(ui->update, SIGNAL(pressed()), this, SLOT(updateInfo()));
    connect(ui->calc, SIGNAL(pressed()), this, SLOT(calculate()));
    connect(ui->node, SIGNAL(pressed()), this, SLOT(updateNet()));
    connect(ui->add, SIGNAL(pressed()), this, SLOT(nodeAdd()));
}

int heightPrevious = -1;
int connectionPrevious = -1;
int volumePrevious = -1;
double rewardPrevious = -1;
double netPawratePrevious = -1;
double pawratePrevious = -1;
double hardnessPrevious = -1;

void StatisticsPage::updateStatistics()
{    
    double pHardness = getHardness();
    int pPawrate = getNetworkPawsPS();
    double pPawrate2 = 0.000;
    int nHeight = pindexBest->nHeight;
    int lPawrate = this->model->getHashrate();
    double lPawrate2 = 0.000;
    double nSubsidy = getReward();
    double nextHardness = GetEstimatedNextHardness();
    int volume = getTotalVolume();
    int peers = this->model->getNumConnections();
    lPawrate2 = ((double)lPawrate / 1000);
    pPawrate2 = ((double)pPawrate / 1000);  
    std::string hash = getAcreHash(nHeight);
    int acreTime = getAcreTime(nHeight);
    int acreInHour = acresInPastHours(1);
    int acreInDay = acresInPastHours(24);
    QString height = QString::number(nHeight);
    QString subsidy = QString::number(nSubsidy, 'f', 6);
    QString hardness = QString::number(pHardness, 'f', 6);
    QString QnextHardness = QString::number(nextHardness, 'f', 6);
    QString pawrate = QString::number(pPawrate2, 'f', 3);
    QString Qlpawrate = QString::number(lPawrate2, 'f', 3);
    QString QPeers = QString::number(peers);
    QString qVolume = QLocale(QLocale::English).toString(volume);
    QString QHash = QString::fromUtf8(hash.c_str());
    QDateTime QRTime = QDateTime::fromTime_t(acreTime);
    QString QTime = QRTime.toString("yyyy:MM:dd hh:mm:ss");
    QString QHour = QString::number(acreInHour);
    QString QDay = QString::number(acreInDay);
    if(nHeight > heightPrevious)
    {
        ui->heightBox->setText("<b><font color=\"green\">" + height + "</font></b>");
    } else {
    ui->heightBox->setText(height);
    }
    
    if(nSubsidy < rewardPrevious)
    {
        ui->rewardBox->setText("<b><font color=\"red\">" + subsidy + "</font></b>");
    } else {
    ui->rewardBox->setText(subsidy);
    }
    
    if(pHardness > hardnessPrevious)
    {
        ui->diffBox->setText("<b><font color=\"green\">" + hardness + "</font></b>");        
    } else if(pHardness < hardnessPrevious) {
        ui->diffBox->setText("<b><font color=\"red\">" + hardness + "</font></b>");
    } else {
        ui->diffBox->setText(hardness);        
    }
    
    if(nextHardness > pHardness)
    {
        ui->nextBox->setText("<b><font color=\"green\">" + QnextHardness + "</font></b>");        
    } else if(nextHardness < pHardness) {
        ui->nextBox->setText("<b><font color=\"red\">" + QnextHardness + "</font></b>"); 
    } else {
        ui->nextBox->setText(QnextHardness);        
    }
    
    if(pPawrate2 > netPawratePrevious)
    {
        ui->pawrateBox->setText("<b><font color=\"green\">" + pawrate + " KH/s</font></b>");             
    } else if(pPawrate2 < netPawratePrevious) {
        ui->pawrateBox->setText("<b><font color=\"red\">" + pawrate + " KH/s</font></b>");        
    } else {
        ui->pawrateBox->setText(pawrate + " KH/s");        
    }
    
    if(lPawrate > pawratePrevious)
    {
        ui->localBox->setText("<b><font color=\"green\">" + Qlpawrate + " KH/s</font></b>");             
    } else if(lPawrate < pawratePrevious) {
        ui->localBox->setText("<b><font color=\"red\">" + Qlpawrate + " KH/s</font></b>");        
    } else {
        ui->localBox->setText(Qlpawrate + " KH/s");      
    }
    
    if(peers > connectionPrevious)
    {
        ui->connectionBox->setText("<b><font color=\"green\">" + QPeers + "</font></b>");             
    } else if(peers < connectionPrevious) {
        ui->connectionBox->setText("<b><font color=\"red\">" + QPeers + "</font></b>");        
    } else {
        ui->connectionBox->setText(QPeers);  
    }

    if(volume > volumePrevious)
    {
        ui->volumeBox->setText("<b><font color=\"green\">" + qVolume + " FOX" + "</font></b>");             
    } else if(volume < volumePrevious) {
        ui->volumeBox->setText("<b><font color=\"red\">" + qVolume + " FOX" + "</font></b>");        
    } else {
        ui->volumeBox->setText(qVolume + " FOX");  
    }
    ui->hashBox->setText(QHash);
    ui->timeBox->setText(QTime);
    ui->hourBox->setText(QHour);
    ui->dayBox->setText(QDay);
    updatePrevious(nHeight, nSubsidy, pHardness, pPawrate2, lPawrate, peers, volume);
}

void StatisticsPage::updatePrevious(int nHeight, double nSubsidy, double pHardness, double pPawrate2, double lPawrate, int peers, int volume)
{
    heightPrevious = nHeight;
    rewardPrevious = nSubsidy;
    hardnessPrevious = pHardness;
    netPawratePrevious = pPawrate2;
    pawratePrevious = lPawrate;
    connectionPrevious = peers;
    volumePrevious = volume;
}

void StatisticsPage::updateInfo()
{
    int netpawrate = getNetworkPawsPS();
    double hardness = getHardness();
    double reward = getReward();
    double profitPerKP = (1000 / (double)netpawrate) * reward * 60 * 24;
    double profitPerMP = (1000000 / (double)netpawrate) * reward * 60 * 24;
    double netpawrate2 = 0.000;
    netpawrate2 = ((double)netpawrate / 1000);
    QString QPawrate = QString::number(netpawrate2, 'f', 3);
    QString QReward = QString::number(reward, 'f', 6);
    QString QHardness = QString::number(hardness, 'f', 6);
    QString QKP = QString::number(profitPerKP, 'f', 6);
    QString QMP = QString::number(profitPerMP, 'f', 6);
    ui->pawrate->setText(QPawrate + " KP/s");
    ui->hardness->setText(QHardness);
    ui->reward->setText(QReward + " FOX");
    ui->KP->setText(QKP + " FOX");
    ui->MP->setText(QMP + " FOX");
}

void StatisticsPage::calculate()
{
    int netpawrate = getNetworkPawsPS();
    int target = ui->target->value();
    int multiplier = pawMultiplier();
    double reward = getReward();
    double final = ((target * multiplier) / (double)netpawrate) * reward * 60 * 24;
    QString QFinal = QString::number(final, 'f', 6);
    ui->end->setText("<b>Expected Profit: " + QFinal + " FOX /day</b>");
    updateInfo();
}

int StatisticsPage::pawMultiplier()
{
    int s = ui->combo->currentIndex();
    int i = 1;
    int ib = 1;
    if(s == 0)
    {
        return 1 ;
    } else {
        while(s > 0)
        {
            s--;
            i = ib * 1000;
            ib = i;
        }
        return i;
    }
}

void StatisticsPage::updateNet()
{
    std::string nodes = getNodeInfo();
    QString QNodes = QString::fromUtf8(nodes.c_str());
    ui->nodes->setText(QNodes);
}

void StatisticsPage::setModel(ClientModel *model)
{
    updateStatistics();
    this->model = model;
    ui->versionLabel->setText(model->formatFullVersion());
    updateInfo();
    updateNet();
}

void StatisticsPage::nodeAdd()
{
    std::string node = ui->addnode->text().toStdString();
    bool s = addnode(node);
    QString m;
    if(s)
    {
        m = "<b><font color=green>Successfully connected to node</font></b>";
    } else {
        m = "<b><font color=red>Node connection failed</font></b>";
    }
    ui->noderesult->setText(m);
    updateNet();
    updateInfo();
    updateStatistics();
}

StatisticsPage::~StatisticsPage()
{
    delete ui;
}