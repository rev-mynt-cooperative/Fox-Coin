#ifndef ACREBROWSER_H
#define ACREBROWSER_H

#include "clientmodel.h"

#include <QWidget>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include <QMap>
#include <QSettings>
#include <QSlider>

namespace Ui {
class AcreBrowser;
}
class ClientModel;

class AcreBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit AcreBrowser(QWidget *parent = 0);
    ~AcreBrowser();
    
    void setModel(ClientModel *model);
    
public slots:
    
    void nextAcre();
    void previousAcre();
    void acreClicked();
    void txClicked();
    void updateExplorer(bool);

private slots:

private:
    Ui::AcreBrowser *ui;
    ClientModel *model;
    
};

#endif // ACREBROWSER_H
