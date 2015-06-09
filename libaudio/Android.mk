ifeq ($(TARGET_BOOTLOADER_BOARD_NAME),motus)

ifneq ($(BUILD_TINY_ANDROID),true)

#$(warning Enabling QCOM audio HW )

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=               \
    AudioPolicyManager.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libmedia

LOCAL_CFLAGS += -DWITH_A2DP

LOCAL_WHOLE_STATIC_LIBRARIES := libaudiopolicy_legacy

LOCAL_MODULE := audio_policy.motus
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

##################################
# libaudio
##################################

include $(CLEAR_VARS)

LOCAL_MODULE := libaudio

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libmedia \
    libhardware_legacy

ifeq ($(TARGET_OS)-$(TARGET_SIMULATOR),linux-true)
    LOCAL_LDLIBS += -ldl
endif

ifneq ($(TARGET_SIMULATOR),true)
LOCAL_SHARED_LIBRARIES += libdl
endif

LOCAL_CFLAGS += -fno-short-enums -DSURF -DMOT_FEATURE_PLATFORM_MSM7K=1

LOCAL_SRC_FILES += AudioHardware.cpp

ifeq ($(strip $(BOARD_USES_QCOM_HARDWARE)), true)
LOCAL_CFLAGS += -DMSM72XX_AUDIO
LOCAL_CFLAGS += -DVOC_CODEC_DEFAULT=0
endif

#LOCAL_LDLIBS += -lpthread

LOCAL_STATIC_LIBRARIES += libaudiohw_legacy

#ifeq ($(BOARD_HAVE_BLUETOOTH),true)
#  LOCAL_SHARED_LIBRARIES += liba2dp
#endif

include $(BUILD_SHARED_LIBRARY)

##################################
# audioHAL
##################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    AudioHal.c \
    AudioHal_Thunks.cpp \

LOCAL_C_INCLUDES := \
    $(call include-path-for, audio-utils)

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libutils \
    libmedia \
    libaudio

LOCAL_MODULE := audio.primary.motus
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)


endif # not BUILD_TINY_ANDROID

endif
