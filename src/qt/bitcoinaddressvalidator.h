// Copyright (c) 2011-2014 The CleanCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CleanCoin_QT_CleanCoinADDRESSVALIDATOR_H
#define CleanCoin_QT_CleanCoinADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class CleanCoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit CleanCoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** CleanCoin address widget validator, checks for a valid CleanCoin address.
 */
class CleanCoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit CleanCoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // CleanCoin_QT_CleanCoinADDRESSVALIDATOR_H
