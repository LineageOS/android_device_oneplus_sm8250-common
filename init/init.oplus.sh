#! /vendor/bin/sh
#
# Copyright (C) 2022 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

if grep -qE "androidboot.rf_version=(11|13|14|15)" /proc/cmdline; then
    setprop vendor.radio.multisim.config dsds
fi
