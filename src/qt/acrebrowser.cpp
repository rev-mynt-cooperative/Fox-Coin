#include "acrebrowser.h"
#include "ui_acrebrowser.h"
#include "main.h"
#include "foxcoinfunction.h"
#include <sstream>
#include <string>

AcreBrowser::AcreBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AcreBrowser)
{
    ui->setupUi(this);
    
    setFixedSize(400, 420);
    
    ui->nextButton->hide();
    ui->previousButton->hide();
    ui->txID->hide();
    ui->txLabel->hide();
    ui->valueLabel->hide();
    ui->valueBox->hide();
    ui->inputLabel->hide();
    ui->inputBox->hide();
    ui->outputLabel->hide();
    ui->outputBox->hide();    
    ui->heightLabel->hide();
    ui->heightLabel_2->hide();
    ui->hashLabel->hide();
    ui->hashBox->hide();
    ui->merkleLabel->hide();
    ui->merkleBox->hide();
    ui->nonceLabel->hide();
    ui->nonceBox->hide();
    ui->bitsLabel->hide();
    ui->bitsBox->hide();
    ui->timeLabel->hide();
    ui->timeBox->hide();
    ui->hardLabel->hide();
    ui->hardBox->hide();
    ui->pawLabel->hide();
    ui->pawBox->hide();
    ui->feesLabel->hide();
    ui->feesBox->hide();
        
    connect(ui->acreButton, SIGNAL(pressed()), this, SLOT(acreClicked()));
    connect(ui->txButton, SIGNAL(pressed()), this, SLOT(txClicked()));
    connect(ui->nextButton, SIGNAL(pressed()), this, SLOT(nextAcre()));
    connect(ui->previousButton, SIGNAL(pressed()), this, SLOT(previousAcre()));
}

void AcreBrowser::updateExplorer(bool acre)
{    
    if(acre)
    {
        ui->txID->hide();
        ui->txLabel->hide();
        ui->valueLabel->hide();
        ui->valueBox->hide();
        ui->inputLabel->hide();
        ui->inputBox->hide();
        ui->outputLabel->hide();
        ui->outputBox->hide();
        ui->heightLabel->show();
        ui->heightLabel_2->show();
        ui->hashLabel->show();
        ui->hashBox->show();
        ui->merkleLabel->show();
        ui->merkleBox->show();
        ui->nonceLabel->show();
        ui->nonceBox->show();
        ui->bitsLabel->show();
        ui->bitsBox->show();
        ui->timeLabel->show();
        ui->timeBox->show();
        ui->hardLabel->show();
        ui->hardBox->show();;
        ui->pawLabel->show();
        ui->pawBox->show();
        ui->nextButton->show();
        ui->previousButton->show();
        ui->feesLabel->hide();
        ui->feesBox->hide(); 
        int height = ui->heightBox->value();
        int Pawrate = getAcrePawrate(height);
        double Pawrate2 = 0.000;
        Pawrate2 = ((double)Pawrate / 1000);  
        std::string hash = getAcreHash(height);
        std::string merkle = getAcreMerkle(height);
        int nBits = getAcrenBits(height);
        int nNonce = getAcreNonce(height);
        int atime = getAcreTime(height);
        double hardness = getAcreHardness(height);
        QString QHeight = QString::number(height);
        QString QHash = QString::fromUtf8(hash.c_str());
        QString QMerkle = QString::fromUtf8(merkle.c_str());
        QString QBits = QString::number(nBits);
        QString QNonce = QString::number(nNonce);
        QString QTime = QString::number(atime);
        QString QHardness = QString::number(hardness, 'f', 6);
        QString QPawrate = QString::number(Pawrate2, 'f', 3);
        ui->heightLabel->setText(QHeight);
        ui->hashBox->setText(QHash);
        ui->merkleBox->setText(QMerkle);
        ui->bitsBox->setText(QBits);
        ui->nonceBox->setText(QNonce);
        ui->timeBox->setText(QTime);     
        ui->hardBox->setText(QHardness);
        ui->pawBox->setText(QPawrate + " KH/s");
    } 
    
    if(acre == false) {
        ui->heightLabel->hide();
        ui->heightLabel_2->hide();
        ui->hashLabel->hide();
        ui->hashBox->hide();
        ui->merkleLabel->hide();
        ui->merkleBox->hide();
        ui->nonceLabel->hide();
        ui->nonceBox->hide();
        ui->bitsLabel->hide();
        ui->bitsBox->hide();
        ui->timeLabel->hide();
        ui->timeBox->hide();
        ui->hardLabel->hide();
        ui->hardBox->hide();
        ui->pawLabel->hide();
        ui->pawBox->hide();
        ui->nextButton->hide();
        ui->previousButton->hide();
        ui->txID->show();
        ui->txLabel->show();
        ui->valueLabel->show();
        ui->valueBox->show();
        ui->inputLabel->show();
        ui->inputBox->show();
        ui->outputLabel->show();
        ui->outputBox->show();
        ui->feesLabel->show();
        ui->feesBox->show();
        std::string txid = ui->txBox->text().toUtf8().constData();
        double value = getTxTotalValue(txid);
        double fees = getTxFees(txid);
        std::string outputs = getOutputs(txid);
        std::string inputs = getInputs(txid);
        QString QValue = QString::number(value, 'f', 6);
        QString QID = QString::fromUtf8(txid.c_str());
        QString QOutputs = QString::fromUtf8(outputs.c_str());
        QString QInputs = QString::fromUtf8(inputs.c_str());
        QString QFees = QString::number(fees, 'f', 6);
        ui->valueBox->setText(QValue + " FOX");
        ui->txID->setText(QID);
        ui->outputBox->setText(QOutputs);
        ui->inputBox->setText(QInputs);
        ui->feesBox->setText(QFees + " FOX");
    }
}

void AcreBrowser::nextAcre()
{
    int height = ui->heightBox->value();
    ui->heightBox->setValue(height + 1);
    updateExplorer(true);
}

void AcreBrowser::previousAcre()
{
    int height = ui->heightBox->value();
    ui->heightBox->setValue(height - 1);
    updateExplorer(true);
}

void AcreBrowser::txClicked()
{
    updateExplorer(false);
}

void AcreBrowser::acreClicked()
{
    updateExplorer(true);
}

void AcreBrowser::setModel(ClientModel *model)
{
    this->model = model;
}

AcreBrowser::~AcreBrowser()
{
    delete ui;
}
