/*
 * Copyright (C) 2022-2023 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::base::Split;
using android::base::Trim;

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char* name, const char* value) {
    size_t valuelen = strlen(value);

    prop_info* pi = (prop_info*)__system_property_find(name);
    if (pi != nullptr) {
        __system_property_update(pi, value, valuelen);
    } else {
        __system_property_add(name, strlen(name), value, valuelen);
    }
}

/*
 * Only for read-only properties. Properties that can be wrote to more
 * than once should be set in a typical init script (e.g. init.oplus.hw.rc)
 * after the original property has been set.
 */
void vendor_load_properties() {
    auto device = GetProperty("ro.product.product.device", "");
    auto rf_version = std::stoi(GetProperty("ro.boot.rf_version", "0"));

    switch (rf_version) {
        case 11: // CN
            if (device == "OnePlus8") {
                OverrideProperty("ro.product.product.model", "IN2010");
            } else if (device == "OnePlus8T") {
                OverrideProperty("ro.product.product.model", "KB2000");
            } else if (device == "OnePlus8Pro") {
                OverrideProperty("ro.product.product.model", "IN2020");
            } else if (device == "OnePlus9R") {
                OverrideProperty("ro.product.product.model", "LE2100");
            }
            break;
        case 12: // TMO
            if (device == "OnePlus8") {
                OverrideProperty("ro.product.product.model", "IN2017");
            } else if (device == "OnePlus8T") {
                OverrideProperty("ro.product.product.model", "KB2007");
            } else if (device == "OnePlus8Pro") {
                OverrideProperty("ro.product.product.model", "IN2027");
            }
            break;
        case 13: // IN
            if (device == "OnePlus8") {
                OverrideProperty("ro.product.product.model", "IN2011");
            } else if (device == "OnePlus8T") {
                OverrideProperty("ro.product.product.model", "KB2001");
            } else if (device == "OnePlus8Pro") {
                OverrideProperty("ro.product.product.model", "IN2021");
            } else if (device == "OnePlus9R") {
                OverrideProperty("ro.product.product.model", "LE2101");
            }
            break;
        case 14: // EU
            if (device == "OnePlus8") {
                OverrideProperty("ro.product.product.model", "IN2013");
            } else if (device == "OnePlus8T") {
                OverrideProperty("ro.product.product.model", "KB2003");
            } else if (device == "OnePlus8Pro") {
                OverrideProperty("ro.product.product.model", "IN2023");
            }
            break;
        case 15: // NA
            if (device == "OnePlus8") {
                OverrideProperty("ro.product.product.model", "IN2015");
            } else if (device == "OnePlus8T") {
                OverrideProperty("ro.product.product.model", "KB2005");
            } else if (device == "OnePlus8Pro") {
                OverrideProperty("ro.product.product.model", "IN2025");
            }
            break;
        default:
            LOG(ERROR) << "Unexpected RF version: " << rf_version;
    }

    if (std::string content; ReadFileToString("/proc/devinfo/ddr_type", &content)) {
        OverrideProperty("ro.boot.ddr_type", Split(Trim(content), "\t").back().c_str());
    }
}
