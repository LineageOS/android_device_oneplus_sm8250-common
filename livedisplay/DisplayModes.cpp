/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "DisplayModesService"

#include "DisplayModes.h"

#include <android-base/logging.h>
#include <android-base/properties.h>

#include <fstream>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_1 {
namespace implementation {

static constexpr const char* kDisplayModeProp = "vendor.display.mode";
static const std::string kModeBasePath = "/sys/class/drm/card0-DSI-1/";
static const std::string kDefaultPath = "/data/vendor/display/default_display_mode";

const std::map<int32_t, DisplayModes::ModeInfo> DisplayModes::kModeMap = {
    {0, {"Standard", "default"}},
    {1, {"DCI P3", "native_display_p3_mode"}},
    {2, {"Wide Color", "native_display_wide_color_mode"}},
    {3, {"sRGB", "native_display_srgb_color_mode"}},
};

DisplayModes::DisplayModes() : mDefaultModeId(0) {
    std::ifstream defaultFile(kDefaultPath);
    std::string value;

    defaultFile >> value;
    LOG(DEBUG) << "Default file read result " << value << " fail " << defaultFile.fail();
    if (defaultFile.fail()) {
        return;
    }

    for (const auto& entry : kModeMap) {
        // Check if default mode is a valid mode
        if (value == std::to_string(entry.first)) {
            mDefaultModeId = entry.first;
            android::base::SetProperty(kDisplayModeProp, entry.second.node);
            break;
        }
    }
}

// Methods from ::vendor::lineage::livedisplay::V2_1::IDisplayModes follow.
Return<void> DisplayModes::getDisplayModes(getDisplayModes_cb resultCb) {
    std::vector<V2_0::DisplayMode> modes;

    for (const auto& entry : kModeMap) {
        modes.push_back({entry.first, entry.second.name});
    }
    resultCb(modes);
    return Void();
}

Return<void> DisplayModes::getCurrentDisplayMode(getCurrentDisplayMode_cb resultCb) {
    int32_t currentModeId = mDefaultModeId;
    std::string value;

    for (const auto& entry : kModeMap) {
        if (entry.first == 0) {
            continue;
        }

        std::ifstream modeFile(kModeBasePath + entry.second.node);
        if (!modeFile.fail()) {
            modeFile >> value;
            if (value == "1") {
                currentModeId = entry.first;
                break;
            }
        }
    }
    resultCb({currentModeId, kModeMap.at(currentModeId).name});
    return Void();
}

Return<void> DisplayModes::getDefaultDisplayMode(getDefaultDisplayMode_cb resultCb) {
    resultCb({mDefaultModeId, kModeMap.at(mDefaultModeId).name});
    return Void();
}

Return<bool> DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    // Disable all modes
    for (const auto& entry : kModeMap) {
        if (entry.first == 0) {
            continue;
        }

        std::ofstream modeFile(kModeBasePath + entry.second.node);
        if (!modeFile.fail()) {
            modeFile << 0;
        }
    }
    const auto iter = kModeMap.find(modeID);
    if (iter == kModeMap.end()) {
        return false;
    }
    if (modeID != 0) {
        std::ofstream modeFile(kModeBasePath + iter->second.node);
        modeFile << 1;
        if (modeFile.fail()) {
            return false;
        }
        android::base::SetProperty(kDisplayModeProp, iter->second.node);
    } else {
        android::base::SetProperty(kDisplayModeProp, "");
    }

    if (makeDefault) {
        std::ofstream defaultFile(kDefaultPath);
        defaultFile << iter->first;
        if (defaultFile.fail()) {
            return false;
        }
        mDefaultModeId = iter->first;
    }

    return true;
}

}  // namespace implementation
}  // namespace V2_1
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
