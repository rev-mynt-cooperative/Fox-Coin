#include "foxcoinunits.h"

#include <QStringList>
#include <QLocale>

FoxcoinUnits::FoxcoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<FoxcoinUnits::Unit> FoxcoinUnits::availableUnits()
{
    QList<FoxcoinUnits::Unit> unitlist;
    unitlist.append(FOX);
    unitlist.append(mFOX);
    unitlist.append(uFOX);
    unitlist.append(nFOX);
    unitlist.append(KFOX);
    unitlist.append(MFOX);
    unitlist.append(GFOX);
    return unitlist;
}

bool FoxcoinUnits::valid(int unit)
{
    switch(unit)
    {
    case FOX:
    case mFOX:
    case uFOX:
    case nFOX:
    case KFOX:
    case MFOX:
    case GFOX:
        return true;
    default:
        return false;
    }
}

QString FoxcoinUnits::name(int unit)
{
    switch(unit)
    {
    case FOX: return QString("FOX");
    case mFOX: return QString("mFOX");
    case uFOX: return QString::fromUtf8("μFOX");
    case nFOX: return QString("nFOX");
    case KFOX: return QString("KFOX");
    case MFOX: return QString("MFOX");
    case GFOX: return QString("GFOX");
    default: return QString("???");
    }
}

QString FoxcoinUnits::description(int unit)
{
    switch(unit)
    {
    case FOX: return QString("FoxCoin");
    case mFOX: return QString("milliFoxCoin (1 / 1,000)");
    case uFOX: return QString("microFoxCoin (1 / 1,000,000)");
    case nFOX: return QString("nanoFoxCoin (1 / 100,000,000)");
    case KFOX: return QString("KiloFoxCoin (1 * 1,000)");
    case MFOX: return QString("MegaFoxCoin (1 * 1,000,000)");
    case GFOX: return QString("GigaFoxCoin (1 * 1,000,000,000)");
    default: return QString("???");
    }
}
//a single unit (.00000001) of FoxCoin is called a "nanoFoxCoin."
qint64 FoxcoinUnits::factor(int unit)
{
    switch(unit)
    {
    case FOX:  return 100000000;
    case mFOX: return 100000;
    case uFOX: return 100;
    case nFOX: return 1;
    case KFOX: return 100000000000;
    case MFOX: return 100000000000000;
    case GFOX: return 100000000000000000;
    default:   return 100000000;
    }
}

int FoxcoinUnits::amountDigits(int unit)
{
    switch(unit)
    {
    case FOX: return 11; // 1,000,000,000 (# digits, without commas)
    case mFOX: return 13; // 1,000,000,000,000
    case uFOX: return 16; // 1,000,000,000,000,000
    case nFOX: return 18; // 100,000,000,000,000,000 
    case KFOX: return 7; // 1,000,000
    case MFOX: return 4; // 1,000
    case GFOX: return 1; // 1
    default: return 0;
    }
}

int FoxcoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case FOX: return 8;
    case mFOX: return 5;
    case uFOX: return 2;
    case nFOX: return 0;
    case KFOX: return 11;
    case MFOX: return 14;
    case GFOX: return 17;
    default: return 0;
    }
}

QString FoxcoinUnits::format(int unit, qint64 n, bool fPlus)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str1 = QString::number(quotient);
    QString quotient_str = QLocale(QLocale::English).toString(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Right-trim excess 0's after the decimal point
    int nTrim = 0;
    for (int i = remainder_str.size()-1; i>=2 && (remainder_str.at(i) == '0'); --i)
        ++nTrim;
    remainder_str.chop(nTrim);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');
    return quotient_str + QString(".") + remainder_str;
}

QString FoxcoinUnits::formatWithUnit(int unit, qint64 amount, bool plussign)
{
    return format(unit, amount, plussign) + QString(" ") + name(unit);
}

bool FoxcoinUnits::parse(int unit, const QString &value, qint64 *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);
    QStringList parts = value.split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    qint64 retvalue = str.toLongLong(&ok);
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

int FoxcoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant FoxcoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}
