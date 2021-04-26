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

static const std::string kModeBasePath = "/sys/class/drm/card0-DSI-1/";
static const std::string kDefaultPath = "/data/vendor/display/default_display_mode";

// Mode ids here must match qdcm display mode ids
const std::map<int32_t, DisplayModes::ModeInfo> DisplayModes::kModeMap = {
    {0,
     {"Standard",
      {
          {"native_display_p3_mode", "0"},
          {"native_display_wide_color_mode", "0"},
          {"native_display_srgb_color_mode", "0"},
          {"native_display_customer_srgb_mode", "0"},
          {"native_display_customer_p3_mode", "0"},
          {"native_display_p3_mode", "1"},
          {"native_display_loading_effect_mode", "1"},
      }}},
    {1,
     {"Natural",
      {
          {"native_display_p3_mode", "0"},
          {"native_display_wide_color_mode", "0"},
          {"native_display_srgb_color_mode", "0"},
          {"native_display_customer_srgb_mode", "0"},
          {"native_display_customer_p3_mode", "0"},
          {"native_display_srgb_color_mode", "1"},
          {"native_display_loading_effect_mode", "0"},
      }}},
    {2,
     {"AMOLED Wide Gamut",
      {
          {"native_display_p3_mode", "0"},
          {"native_display_wide_color_mode", "0"},
          {"native_display_srgb_color_mode", "0"},
          {"native_display_customer_srgb_mode", "0"},
          {"native_display_customer_p3_mode", "0"},
          {"native_display_wide_color_mode", "1"},
          {"native_display_loading_effect_mode", "1"},
      }}},
    {3,
     {"sRGB",
      {
          {"native_display_p3_mode", "0"},
          {"native_display_wide_color_mode", "0"},
          {"native_display_srgb_color_mode", "0"},
          {"native_display_customer_srgb_mode", "0"},
          {"native_display_customer_p3_mode", "0"},
          {"native_display_customer_srgb_mode", "1"},
          {"native_display_loading_effect_mode", "0"},
      }}},
    {4,
     {"DCI_P3",
      {
          {"native_display_p3_mode", "0"},
          {"native_display_wide_color_mode", "0"},
          {"native_display_srgb_color_mode", "0"},
          {"native_display_customer_srgb_mode", "0"},
          {"native_display_customer_p3_mode", "0"},
          {"native_display_customer_p3_mode", "1"},
          {"native_display_loading_effect_mode", "0"},
      }}},
};

DisplayModes::DisplayModes(std::shared_ptr<V2_0::sdm::SDMController> controller)
    : mController(std::move(controller)), mDefaultModeId(0) {
    std::ifstream defaultFile(kDefaultPath);

    defaultFile >> mDefaultModeId;
    LOG(DEBUG) << "Default file read result " << mDefaultModeId << " fail " << defaultFile.fail();

    setDisplayMode(mDefaultModeId, false);
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
    std::ifstream defaultFile(kDefaultPath);

    int32_t currentModeId = mDefaultModeId;
    defaultFile >> currentModeId;

    resultCb({currentModeId, kModeMap.at(currentModeId).name});
    return Void();
}

Return<void> DisplayModes::getDefaultDisplayMode(getDefaultDisplayMode_cb resultCb) {
    resultCb({mDefaultModeId, kModeMap.at(mDefaultModeId).name});
    return Void();
}

Return<bool> DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    const auto iter = kModeMap.find(modeID);
    if (iter == kModeMap.end()) {
        return false;
    }
    for (const auto& [name, value] : iter->second.commands) {
        std::ofstream file(kModeBasePath + name);
        file << value;
        if (file.fail()) {
            LOG(ERROR) << "Failed to write to " << (kModeBasePath + name);
        }
    }
    mController->setActiveDisplayMode(iter->first);
    if (makeDefault) {
        std::ofstream defaultFile(kDefaultPath);
        defaultFile << iter->first;
        if (defaultFile.fail()) {
            return false;
        }
        mController->setDefaultDisplayMode(iter->first);
        mDefaultModeId = iter->first;
    }
    if (mOnDisplayModeSet) {
        mOnDisplayModeSet();
    }
    return true;
}

}  // namespace implementation
}  // namespace V2_1
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
