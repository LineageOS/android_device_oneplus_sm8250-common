#
# Copyright 2020 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := \
    libbase \
    liblog \
    libcutils \
    libtinyalsa \
    libtinycompress \
    libaudioroute \
    libdl \
    libaudioutils \
    libhwbinder \
    libhidlbase \
    libprocessgroup \
    libutils \
    audio.primary.$(TARGET_BOARD_PLATFORM)

LOCAL_C_INCLUDES += \
    external/tinyalsa/include \
    external/tinycompress/include \
    hardware/libhardware/include \
    system/media/audio_utils/include \
    $(call include-path-for, audio-effects) \
    $(call include-path-for, audio-route) \
    $(call project-path-for,qcom-audio)/hal \
    $(call project-path-for,qcom-audio)/hal/msm8974 \
    $(call project-path-for,qcom-audio)/hal/audio_extn \
    $(call project-path-for,qcom-audio)/hal/voice_extn

LOCAL_HEADER_LIBRARIES := generated_kernel_headers
LOCAL_SRC_FILES := audio_amplifier.c
LOCAL_MODULE := audio_amplifier.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true

include $(BUILD_SHARED_LIBRARY)
