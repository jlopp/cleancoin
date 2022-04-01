// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The CleanCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CleanCoin_CONSENSUS_PARAMS_H
#define CleanCoin_CONSENSUS_PARAMS_H

#include <uint256.h>
#include <limits>

namespace Consensus {

enum DeploymentPos
{
    DEPLOYMENT_TESTDUMMY,
    DEPLOYMENT_TAPROOT, // Deployment of Schnorr/Taproot (BIPs 340-342)
    // NOTE: Also add new deployments to VersionBitsDeploymentInfo in versionbits.cpp
    MAX_VERSION_BITS_DEPLOYMENTS
};

/**
 * Struct for each individual consensus rule change using BIP9.
 */
struct BIP9Deployment {
    /** Bit position to select the particular bit in nVersion. */
    int bit;
    /** Start MedianTime for version bits miner confirmation. Can be a date in the past */
    int64_t nStartTime;
    /** Timeout/expiry MedianTime for the deployment attempt. */
    int64_t nTimeout;

    /** Constant for nTimeout very far in the future. */
    static constexpr int64_t NO_TIMEOUT = std::numeric_limits<int64_t>::max();

    /** Special value for nStartTime indicating that the deployment is always active.
     *  This is useful for testing, as it means tests don't need to deal with the activation
     *  process (which takes at least 3 BIP9 intervals). Only tests that specifically test the
     *  behaviour during activation cannot use this. */
    static constexpr int64_t ALWAYS_ACTIVE = -1;
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /* Block hash that is excepted from BIP16 enforcement */
    uint256 BIP16Exception;
    /** Block height and hash at which BIP34 becomes active */
    int BIP34Height;
    uint256 BIP34Hash;
    /** Block height at which BIP65 becomes active */
    int BIP65Height;
    /** Block height at which BIP66 becomes active */
    int BIP66Height;
    /** Block height at which CSV (BIP68, BIP112 and BIP113) becomes active */
    int CSVHeight;
    /** Block height at which Segwit (BIP141, BIP143 and BIP147) becomes active.
     * Note that segwit v0 script rules are enforced on all blocks except the
     * BIP 16 exception blocks. */
    int SegwitHeight;
    /** Don't warn about unknown BIP 9 activations below this height.
     * This prevents us from warning about the CSV and segwit activations. */
    int MinBIP9WarningHeight;
    /** The size of the step going towards reward matching - rewards from
     * both chains, CleanCoin and CleanCoin are coming in sync with steps of this size. */
    int BPSRewardMatchStep;
    /** Block height at which BPSRewardMatch becomes active - rewards from
     * both chains, CleanCoin and CleanCoin are in sync as of this height. */
    int BPSRewardMatchHeight;
    /** Block height at which BPSDiffAdj becomes active - difficulty adjustment
     * formula is changed so that block times are more reliable. */
    int BPSDiffAdjHeight;
    /** Block height at which BPSColdStakeEnable becomes active - cold staking
     * will be enabled, allowing wallets to use delegations for staking. */
    int BPSColdStakeEnableHeight;
    /**
     * Minimum blocks including miner confirmation of the total of 2016 blocks in a retargeting period,
     * (nPowTargetTimespan / nPowTargetSpacing) which is also used for BIP9 deployments.
     * Examples: 1916 for 95%, 1512 for testchains.
     */
    uint32_t nRuleChangeActivationThreshold;
    uint32_t nMinerConfirmationWindow;
    BIP9Deployment vDeployments[MAX_VERSION_BITS_DEPLOYMENTS];
    /** Proof of work parameters */
    uint256 powLimit;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nPowTargetSpacing;
    int64_t nPowTargetTimespan;
    int64_t DifficultyAdjustmentInterval(const int height) const
    {
        int64_t targetTimeSpan;
        if (height < BPSDiffAdjHeight) {
            targetTimeSpan = nPowTargetSpacing;
        } else {
            targetTimeSpan = nPowTargetTimespan;
        }

        return targetTimeSpan / nPowTargetSpacing;
    }
    /** The best chain should have at least this much work */
    uint256 nMinimumChainWork;
    /** By default assume that the signatures in ancestors of this block are valid */
    uint256 defaultAssumeValid;
    /** Proof of stake parameters */
    uint256 posLimit;
    bool fPoSNoRetargeting;
    int nLastPOWBlock;
    int nFirstMPoSBlock;
    int nLastMPoSBlock;
    int nMPoSRewardRecipients;
    int nEnableHeaderSignatureHeight;
    /** Block sync-checkpoint span*/
    int nCheckpointSpan;

    /**
     * If true, witness commitments contain a payload equal to a CleanCoin Script solution
     * to the signet challenge. See BIP325.
     */
    bool signet_blocks{false};
    std::vector<uint8_t> signet_challenge;
};
} // namespace Consensus

#endif // CleanCoin_CONSENSUS_PARAMS_H
