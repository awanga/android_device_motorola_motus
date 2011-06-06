$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_us_supl.mk)

$(call inherit-product-if-exists, vendor/motorola/motus/motus-vendor.mk)

DEVICE_PACKAGE_OVERLAYS += device/motorola/motus/overlay


ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := device/motorola/motus/kernel
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel

$(call inherit-product, build/target/product/full.mk)

PRODUCT_NAME := generic_motus
PRODUCT_DEVICE := motus