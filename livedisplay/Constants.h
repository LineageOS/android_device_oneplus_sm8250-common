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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_CONSTANTS_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_CONSTANTS_H

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

#define DPPS_BUF_SIZE 64

#define FOSS_PROPERTY "ro.vendor.display.foss"
#define FOSS_ON "foss:on"
#define FOSS_OFF "foss:off"

#define COLOR_BALANCE_FEATURE 3
#define DISPLAY_MODES_FEATURE 4
#define PICTURE_ADJUSTMENT_FEATURE 1

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_CONSTANTS_H
