// Copyright (c) 2020 The CleanCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CleanCoin_TEST_UTIL_VALIDATION_H
#define CleanCoin_TEST_UTIL_VALIDATION_H

#include <validation.h>

struct TestChainState : public CChainState {
    /** Reset the ibd cache to its initial state */
    void ResetIbd();
    /** Toggle IsInitialBlockDownload from true to false */
    void JumpOutOfIbd();
};

#endif // CleanCoin_TEST_UTIL_VALIDATION_H
