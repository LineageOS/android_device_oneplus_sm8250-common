#! /vendor/bin/sh
#
# Copyright (C) 2022 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

if grep -q simcardnum.doublesim=1 /proc/cmdline; then
    setprop vendor.radio.multisim.config dsds
fi
