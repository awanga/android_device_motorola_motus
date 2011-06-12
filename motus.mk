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

#
# This is the product configuration for a generic GSM passion,
# not specialized for any geography.
#

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_us_supl.mk)

## (1) First, the most specific values, i.e. the aspects that are specific to GSM

PRODUCT_COPY_FILES += \
    device/motorola/motus/init.motus.rc:root/init.motus.rc \
    device/motorola/motus/ueventd.motus.rc:root/ueventd.motus.rc \
    device/motorola/motus/charge_only_mode:root/sbin/charge_only_mode

## (3)  Finally, the least specific parts, i.e. the non-GSM-specific aspects
PRODUCT_PROPERTY_OVERRIDES += \
        ro.com.android.wifi-watchlist=GoogleGuest \
        ro.error.receiver.system.apps=com.google.android.feedback \
        ro.setupwizard.enterprise_mode=1 \
        ro.com.google.clientidbase=android-att-us \
        ro.com.google.locationfeatures=1 \
        ro.url.legal=http://www.google.com/intl/%s/mobile/android/basic/phone-legal.html \
        ro.url.legal.android_privacy=http://www.google.com/intl/%s/mobile/android/basic/privacy.html \
        ro.config.vc_call_vol_steps=7 \
        ro.telephony.call_ring.multiple=false \
        ro.telephony.call_ring.delay=1000 \
        ro.url.safetylegal=http://www.motorola.com/staticfiles/Support/legal/?model=MB300 \
        ro.setupwizard.enable_bypass=1 \
        dalvik.vm.lockprof.threshold=500 \
        dalvik.vm.dexopt-flags=m=y \
        debug.sf.hw=1 \
        ro.media.dec.aud.wma.enabled=1 \
        ro.media.dec.vid.wmv.enabled=1

DEVICE_PACKAGE_OVERLAYS += device/motorola/motus/overlay

PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/base/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/base/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/base/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/base/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/base/data/etc/android.hardware.touchscreen.multitouch.distinct.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml

PRODUCT_PACKAGES += \
    librs_jni \
    dspexec \
    libbridge \
    gralloc.msm7k \
    copybit.motus \
    librpc \
    libstagefrighthw \
    libOmxCore \
    libmm-omxcore \
    libloc_api-rpc \
    gps.msm7k \
    sensors.motus \
    lights.motus \
    Torch \
    libcamera \
    libaudio \
    rzscontrol \
    dexpreopt \
    setrecovery

DISABLE_DEXPREOPT := true

# we have enough storage space to hold precise GC data
PRODUCT_TAGS += dalvik.gc.type-precise

# Passion uses high-density artwork where available
PRODUCT_LOCALES += mdpi

PRODUCT_COPY_FILES += \
    device/motorola/motus/vold.fstab:system/etc/vold.fstab

PRODUCT_COPY_FILES += \
    device/motorola/motus/btenable.sh:system/bin/btenable.sh \
    device/motorola/motus/kblight_enable.sh:system/bin/kblight_enable.sh \
    device/motorola/motus/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
    device/motorola/motus/dhcpcd.conf:system/etc/dhcpcd/dhcpcd.conf \
    device/motorola/motus/saveconfig.cfg:system/etc/touchpad/50/saveconfig.cfg \
    device/motorola/motus/touchpad.cfg:system/etc/touchpad/50/touchpad.cfg \
    device/motorola/motus/touchpad.enc:system/etc/touchpad/50/touchpad.enc

ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := device/motorola/motus/kernel
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    device/motorola/motus/setrecovery/prebuilt:recovery/root/sbin/setrecovery

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel \
    device/motorola/motus/dhd.ko:system/lib/dhd.ko

$(call inherit-product-if-exists, vendor/motorola/motus/motus-vendor.mk)

PRODUCT_COPY_FILES += \
    device/motorola/motus/media_profiles.xml:/system/etc/media_profiles.xml \
    device/motorola/motus/sysctl.conf:system/etc/sysctl.conf

$(call inherit-product, build/target/product/full_base.mk)

PRODUCT_NAME := generic_motus
PRODUCT_DEVICE := motus
