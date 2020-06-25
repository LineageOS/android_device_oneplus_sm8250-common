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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_TYPES_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_TYPES_H

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

struct sdm_feature_version {
    uint8_t x, y;
    uint16_t z;
};

struct sdm_disp_mode {
    int32_t id;
    int32_t type;
    int32_t len;
    char* name;
};

struct hsic_data {
    int32_t hue;
    float saturation;
    float intensity;
    float contrast;
    float saturationThreshold;
};

struct hsic_config {
    uint32_t unused;
    hsic_data data;
};

struct hsic_int_range {
    int32_t max;
    int32_t min;
    uint32_t step;
};

struct hsic_float_range {
    float max;
    float min;
    float step;
};

struct hsic_ranges {
    uint32_t unused;
    struct hsic_int_range hue;
    struct hsic_float_range saturation;
    struct hsic_float_range intensity;
    struct hsic_float_range contrast;
    struct hsic_float_range saturationThreshold;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_TYPES_H
