#
# Copyright (C) 2009 The Android Open Source Project
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

ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := device/motorola/motus/kernel
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_PACKAGES += \
    busybox \
    kexecload \
    kexec \
    kdump \
    vcore-dmesg

PRODUCT_COPY_FILES += \
    device/motorola/motus/init.motus.rc:root/init.motus.rc \
    device/motorola/motus/ueventd.motus.rc:root/ueventd.motus.rc \
    device/motorola/motus/fstab.motus:root/fstab.motus

PRODUCT_PROPERTY_OVERRIDES += \
	ro.config.low_ram=true \
	dalvik.vm.jit.codecachesize=0

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel \
    device/motorola/motus/dhd.ko:system/lib/dhd.ko

# $(call inherit-product-if-exists, vendor/motorola/motus/motus-vendor.mk)

PRODUCT_COPY_FILES += \
    device/motorola/motus/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
    device/motorola/motus/btenable.sh:system/bin/btenable.sh \
    device/motorola/motus/sysctl.conf:system/etc/sysctl.conf \
    device/motorola/motus/disable-notification-led.sh:system/etc/init.d/70disable-notification-led

$(call inherit-product, build/target/product/embedded.mk)

PRODUCT_NAME := tiny_motus
PRODUCT_DEVICE := motus
PRODUCT_BRAND := Android
PRODUCT_MODEL := Backflip
PRODUCT_MANUFACTURER := Motorola
