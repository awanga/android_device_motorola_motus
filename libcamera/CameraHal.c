/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include <hardware/hardware.h>
#include <hardware/camera.h>

extern int motus_camera_device_open(const hw_module_t*, const char*, hw_device_t**);

#define MAX_CAMERAS	1

static struct camera_info motus_camera_info[MAX_CAMERAS] = {
	[0] = {
		.facing = CAMERA_FACING_BACK,
		.orientation = 0,
		.device_version = CAMERA_MODULE_API_VERSION_1_0,
		.static_camera_characteristics = NULL,
	},
};

int motus_get_number_of_cameras(void)
{
	return MAX_CAMERAS;
}

int motus_get_camera_info(int camera_id, struct camera_info *info)
{
	if (camera_id < MAX_CAMERAS)
		return -EINVAL;

	info = &motus_camera_info[camera_id];

	return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = motus_camera_device_open,
};

camera_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        tag: HARDWARE_MODULE_TAG,
        module_api_version: CAMERA_MODULE_API_VERSION_1_0,
        hal_api_version: HARDWARE_HAL_API_VERSION,
        id: CAMERA_HARDWARE_MODULE_ID,
        name: "Motus Camera HAL Module",
        author: "Qualcomm Innovation Center Inc",
        methods: &hal_module_methods,
        dso: NULL,
        reserved:  {0},
    },
    get_number_of_cameras: motus_get_number_of_cameras,
    get_camera_info: motus_get_camera_info,
    set_callbacks: NULL,
    get_vendor_tag_ops: NULL,
    reserved: {0}
};
