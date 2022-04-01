// Copyright (c) 2019 The CleanCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CleanCoin_NET_TYPES_H
#define CleanCoin_NET_TYPES_H

#include <map>

class CBanEntry;
class CSubNet;

using banmap_t = std::map<CSubNet, CBanEntry>;

#endif // CleanCoin_NET_TYPES_H
