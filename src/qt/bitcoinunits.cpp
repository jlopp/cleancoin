// Copyright (c) 2011-2019 The CleanCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/CleanCoinunits.h>

#include <QStringList>

#include <cassert>

CleanCoinUnits::CleanCoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<CleanCoinUnits::Unit> CleanCoinUnits::availableUnits()
{
    QList<CleanCoinUnits::Unit> unitlist;
    unitlist.append(BPS);
    unitlist.append(mBPS);
    unitlist.append(uBPS);
    unitlist.append(SAT);
    return unitlist;
}

bool CleanCoinUnits::valid(int unit)
{
    switch(unit)
    {
    case BPS:
    case mBPS:
    case uBPS:
    case SAT:
        return true;
    default:
        return false;
    }
}

QString CleanCoinUnits::longName(int unit)
{
    switch(unit)
    {
    case BPS: return QString("BPS");
    case mBPS: return QString("mBPS");
    case uBPS: return QString::fromUtf8("µBPS (bits)");
    case SAT: return QString("Satoshi (sat)");
    default: return QString("???");
    }
}

QString CleanCoinUnits::shortName(int unit)
{
    switch(unit)
    {
    case uBPS: return QString::fromUtf8("bits");
    case SAT: return QString("sat");
    default: return longName(unit);
    }
}

QString CleanCoinUnits::description(int unit)
{
    switch(unit)
    {
    case BPS: return QString("CleanCoins");
    case mBPS: return QString("Milli-CleanCoins (1 / 1" THIN_SP_UTF8 "000)");
    case uBPS: return QString("Micro-CleanCoins (bits) (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
    case SAT: return QString("Satoshi (sat) (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
    default: return QString("???");
    }
}

qint64 CleanCoinUnits::factor(int unit)
{
    switch(unit)
    {
    case BPS: return 100000000;
    case mBPS: return 100000;
    case uBPS: return 100;
    case SAT: return 1;
    default: return 100000000;
    }
}

int CleanCoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case BPS: return 8;
    case mBPS: return 5;
    case uBPS: return 2;
    case SAT: return 0;
    default: return 0;
    }
}

QString CleanCoinUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators, bool justify)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    QString quotient_str = QString::number(quotient);
    if (justify) quotient_str = quotient_str.rightJustified(16 - num_decimals, ' ');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == SeparatorStyle::ALWAYS || (separators == SeparatorStyle::STANDARD && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    if (num_decimals > 0) {
        qint64 remainder = n_abs % coin;
        QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');
        return quotient_str + QString(".") + remainder_str;
    } else {
        return quotient_str;
    }
}


// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString CleanCoinUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + shortName(unit);
}

QString CleanCoinUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

QString CleanCoinUnits::formatWithPrivacy(int unit, const CAmount& amount, SeparatorStyle separators, bool privacy)
{
    assert(amount >= 0);
    QString value;
    if (privacy) {
        value = format(unit, 0, false, separators, true).replace('0', '#');
    } else {
        value = format(unit, amount, false, separators, true);
    }
    return value + QString(" ") + shortName(unit);
}

bool CleanCoinUnits::parse(int unit, const QString &value, CAmount *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

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
    CAmount retvalue(str.toLongLong(&ok));
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString CleanCoinUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (CleanCoinUnits::valid(unit))
    {
        amountTitle += " ("+CleanCoinUnits::shortName(unit) + ")";
    }
    return amountTitle;
}

QString CleanCoinUnits::getDelegatedColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Delegated");
    if (CleanCoinUnits::valid(unit))
    {
        amountTitle += " ("+CleanCoinUnits::shortName(unit) + ")";
    }
    return amountTitle;
}

QString CleanCoinUnits::formatInt(const int64_t &nIn, bool fPlus, CleanCoinUnits::SeparatorStyle separators)
{
    qint64 n = (qint64)nIn;
    qint64 quotient = (n > 0 ? n : -n);
    QString quotient_str = QString::number(quotient);

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == SeparatorStyle::ALWAYS || (separators == SeparatorStyle::STANDARD && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    return quotient_str;
}

int CleanCoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant CleanCoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(longName(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount CleanCoinUnits::maxMoney()
{
    return MAX_MONEY;
}
