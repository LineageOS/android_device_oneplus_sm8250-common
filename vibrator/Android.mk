LOCAL_PATH := vendor/qcom/opensource/vibrator

include $(CLEAR_VARS)

LOCAL_MODULE := vendor.qti.hardware.vibrator.service.oneplus_kona
LOCAL_MODULE_TAGS := optional

LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/vendor_overlay/$(PRODUCT_TARGET_VNDK_VERSION)/bin
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_STEM := vendor.qti.hardware.vibrator.service

LOCAL_CFLAGS += \
    -Wall \
    -Werror

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/aidl/include

LOCAL_SRC_FILES := \
    aidl/service.cpp \
    aidl/Vibrator.cpp \
    effect/effect.cpp

LOCAL_SHARED_LIBRARIES := \
    android.hardware.vibrator-ndk_platform \
    libbase \
    libbinder_ndk \
    libcutils \
    liblog \
    libutils

include $(BUILD_EXECUTABLE)
