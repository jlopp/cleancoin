#!/usr/bin/env bash
# Copyright (c) 2016-2019 The CleanCoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

CleanCoinD=${CleanCoinD:-$BINDIR/CleanCoind}
CleanCoinCLI=${CleanCoinCLI:-$BINDIR/CleanCoin-cli}
CleanCoinTX=${CleanCoinTX:-$BINDIR/CleanCoin-tx}
WALLET_TOOL=${WALLET_TOOL:-$BINDIR/CleanCoin-wallet}
CleanCoinQT=${CleanCoinQT:-$BINDIR/qt/CleanCoin-qt}

[ ! -x $CleanCoinD ] && echo "$CleanCoinD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
read -r -a BPSVER <<< "$($CleanCoinCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }')"

# Create a footer file with copyright content.
# This gets autodetected fine for CleanCoind if --version-string is not set,
# but has different outcomes for CleanCoin-qt and CleanCoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$CleanCoinD --version | sed -n '1!p' >> footer.h2m

for cmd in $CleanCoinD $CleanCoinCLI $CleanCoinTX $WALLET_TOOL $CleanCoinQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BPSVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BPSVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
