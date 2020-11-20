LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := android.hardware.power-service.oneplus_kona
LOCAL_MODULE_TAGS := optional

LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/vendor_overlay/$(PRODUCT_TARGET_VNDK_VERSION)/bin
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_STEM := android.hardware.power-service

LOCAL_CFLAGS += \
    -Wno-unused-parameter \
    -Wno-unused-variable

ifeq ($(TARGET_USES_INTERACTION_BOOST),true)
    LOCAL_CFLAGS += -DINTERACTION_BOOST
endif

LOCAL_C_INCLUDES := \
    external/libxml2/include \
    external/icu/icu4c/source/common

LOCAL_HEADER_LIBRARIES += \
    libhardware_headers \
    libutils_headers

LOCAL_SRC_FILES := \
    hint-data.c \
    list.c \
    main.cpp \
    metadata-parser.c \
    power-common.c \
    Power.cpp \
    powerhintparser.c \
    utils.c

LOCAL_REQUIRED_MODULES := \
    android.hardware.power-service.oneplus_kona.rc

LOCAL_SHARED_LIBRARIES := \
    android.hardware.power-ndk_platform \
    libbase \
    libbinder_ndk \
    libcutils \
    libdl \
    liblog \
    libutils \
    libxml2

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_MODULE := android.hardware.power-service.oneplus_kona.rc
LOCAL_MODULE_TAGS  := optional
LOCAL_MODULE_CLASS := ETC

LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/vendor_overlay/$(PRODUCT_TARGET_VNDK_VERSION)/etc/init
LOCAL_MODULE_STEM := android.hardware.power-service.rc

LOCAL_SRC_FILES := android.hardware.power-service.rc

include $(BUILD_PREBUILT)
