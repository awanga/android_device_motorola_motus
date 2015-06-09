ifeq ($(TARGET_BOOTLOADER_BOARD_NAME),motus)
BUILD_LIBCAMERA:= true
ifeq ($(BUILD_LIBCAMERA),true)

# When zero we link against libmmcamera; when 1, we dlopen libmmcamera.
DLOPEN_LIBMMCAMERA:=1

ifneq ($(BUILD_TINY_ANDROID),true)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS:=optional

LOCAL_SRC_FILES:= QualcommCameraHardware.cpp exifwriter.c jdatadst.cpp jpegConvert.cpp

LOCAL_CFLAGS:= -DDLOPEN_LIBMMCAMERA=$(DLOPEN_LIBMMCAMERA)

LOCAL_C_INCLUDES+= \
        frameworks/native/include/media/hardware \
        frameworks/av/services/camera/libcameraservice \
        frameworks/av/include/camera \
        system/media/camera/include \
        vendor/qcom/proprietary/mm-camera/common \
        vendor/qcom/proprietary/mm-camera/apps/appslib \
        external/jhead \
        external/jpeg \
        vendor/qcom/proprietary/mm-camera/jpeg/inc

LOCAL_SHARED_LIBRARIES:= libbinder libutils libcamera_client libcameraservice liblog libjpeg libjhead

ifneq ($(DLOPEN_LIBMMCAMERA),1)
LOCAL_SHARED_LIBRARIES+= libmmcamera libmm-qcamera-tgt
else
LOCAL_SHARED_LIBRARIES+= libdl libexif
endif

LOCAL_MODULE:= libcamera
include $(BUILD_SHARED_LIBRARY)

##################################
# camera HAL
##################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    CameraHal.c \
    CameraHWI.cpp \

LOCAL_C_INCLUDES := \
    frameworks/native/include/media/hardware \
    frameworks/av/services/camera/libcameraservice \
    frameworks/av/include/camera \
    system/media/camera/include

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libutils \
    libcameraservice \
    libcamera_client \
    libcamera

LOCAL_MODULE := camera.motus
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

endif # BUILD_TINY_ANDROID
endif # BUILD_LIBCAMERA
endif
