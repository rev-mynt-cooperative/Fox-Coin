#include "bitcoinunits.h"

#include <QStringList>
#include <QLocale>

BitcoinUnits::BitcoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<BitcoinUnits::Unit> BitcoinUnits::availableUnits()
{
    QList<BitcoinUnits::Unit> unitlist;
    unitlist.append(BTC);
    unitlist.append(mBTC);
    unitlist.append(uBTC);
    unitlist.append(nBTC);
    unitlist.append(KBTC);
    unitlist.append(MBTC);
    unitlist.append(GBTC);
    return unitlist;
}

bool BitcoinUnits::valid(int unit)
{
    switch(unit)
    {
    case BTC:
    case mBTC:
    case uBTC:
    case nBTC:
    case KBTC:
    case MBTC:
    case GBTC:
        return true;
    default:
        return false;
    }
}

QString BitcoinUnits::name(int unit)
{
    switch(unit)
    {
    case BTC: return QString("FOX");
    case mBTC: return QString("mFOX");
    case uBTC: return QString::fromUtf8("μFOX");
    case nBTC: return QString("nFOX");
    case KBTC: return QString("KFOX");
    case MBTC: return QString("MFOX");
    case GBTC: return QString("GFOX");
    default: return QString("???");
    }
}

QString BitcoinUnits::description(int unit)
{
    switch(unit)
    {
    case BTC: return QString("FoxCoin");
    case mBTC: return QString("milliFoxCoin (1 / 1,000)");
    case uBTC: return QString("microFoxCoin (1 / 1,000,000)");
    case nBTC: return QString("nanoFoxCoin (1 / 100,000,000)");
    case KBTC: return QString("KiloFoxCoin (1 * 1,000)");
    case MBTC: return QString("MegaFoxCoin (1 * 1,000,000)");
    case GBTC: return QString("GigaFoxCoin (1 * 1,000,000,000)");
    default: return QString("???");
    }
}
//a single unit (.00000001) of FoxCoin is called a "nanoFoxCoin."
qint64 BitcoinUnits::factor(int unit)
{
    switch(unit)
    {
    case BTC:  return 100000000;
    case mBTC: return 100000;
    case uBTC: return 100;
    case nBTC: return 1;
    case KBTC: return 100000000000;
    case MBTC: return 100000000000000;
    case GBTC: return 100000000000000000;
    default:   return 100000000;
    }
}

int BitcoinUnits::amountDigits(int unit)
{
    switch(unit)
    {
    case BTC: return 11; // 1,000,000,000 (# digits, without commas)
    case mBTC: return 13; // 1,000,000,000,000
    case uBTC: return 16; // 1,000,000,000,000,000
    case nBTC: return 18; // 100,000,000,000,000,000 
    case KBTC: return 7; // 1,000,000
    case MBTC: return 4; // 1,000
    case GBTC: return 1; // 1
    default: return 0;
    }
}

int BitcoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case BTC: return 8;
    case mBTC: return 5;
    case uBTC: return 2;
    case nBTC: return 0;
    case KBTC: return 11;
    case MBTC: return 14;
    case GBTC: return 17;
    default: return 0;
    }
}

QString BitcoinUnits::format(int unit, qint64 n, bool fPlus)
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

QString BitcoinUnits::formatWithUnit(int unit, qint64 amount, bool plussign)
{
    return format(unit, amount, plussign) + QString(" ") + name(unit);
}

bool BitcoinUnits::parse(int unit, const QString &value, qint64 *val_out)
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

int BitcoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant BitcoinUnits::data(const QModelIndex &index, int role) const
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
