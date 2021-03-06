#!/bin/bash
# MFD & LED brightness tests
#
# Copyright (C) 2012-2018 Nirenjan Krishnan (nirenjan@nirenjan.org)
#
# SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0

source $(dirname $0)/../common_infra.sh

TEST_SUITE_ID="libx52 MFD & LED brightness tests"

brightness_test()
{
    local unit=$(echo $1 | tr a-z A-Z)
    local bri=$(printf '0x%04x' $2)
    local index="\$X52_${unit}_BRIGHTNESS_INDEX"
    TEST_ID="Test setting $unit brightness to $bri"

    expect_pattern `eval echo $index` $bri

    $X52CLI bri $unit $bri

    verify_output
}

for unit in mfd led
do
    for bri in $(seq 0 128)
    do
        brightness_test $unit $bri
    done
done

verify_test_suite

