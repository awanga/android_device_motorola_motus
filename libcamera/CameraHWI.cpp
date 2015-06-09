/*
 * Copyright (C) 2011 The Linux Foundation
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

#include <camera/CameraParameters.h>

#include "QualcommCameraHardware.h"

namespace android
{

static char param_str[255];

struct motus_camera_device {
	camera_device_t device;
	camera_device_ops_t ops;
	QualcommCameraHardware* hw;

	int metadata_store_flag;

	/*struct preview_stream_ops window;*/
};

static int cam_set_preview_window(struct camera_device *device, struct preview_stream_ops *window)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	/*if (window)
		memcpy(&cdev->window, window, sizeof(struct preview_stream_ops));*/

	return 0;
}

static void cam_set_callbacks(struct camera_device *device,
                             camera_notify_callback notify_cb,
							 camera_data_callback data_cb,
							 camera_data_timestamp_callback data_cb_timestamp,
							 camera_request_memory get_memory,
							 void *user)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	cdev->hw->setCallbacks(reinterpret_cast<notify_callback>(notify_cb),
	                       reinterpret_cast<data_callback>(data_cb),
	                       reinterpret_cast<data_callback_timestamp>(data_cb_timestamp),
	                       user);
}

static void cam_enable_msg_type(struct camera_device *device, int32_t msg_type)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	cdev->hw->enableMsgType(msg_type);
}

static void cam_disable_msg_type(struct camera_device *device, int32_t msg_type)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	cdev->hw->disableMsgType(msg_type);
}

static int cam_msg_type_enabled(struct camera_device *device, int32_t msg_type)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->msgTypeEnabled(msg_type);
}

static int cam_start_preview(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->startPreview();
}

static void cam_stop_preview(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	cdev->hw->stopPreview();
}

static int cam_preview_enabled(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->previewEnabled();
}

static int cam_store_meta_data_in_buffers(struct camera_device *device, int enable)
{
	return -ENOSYS;
}

static int cam_start_recording(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->startRecording();
}

static void cam_stop_recording(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	cdev->hw->stopRecording();
}

static int cam_recording_enabled(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->recordingEnabled();
}

static void cam_release_recording_frame(struct camera_device *device, const void *opaque)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);
	sp<IMemory> mem; /* unused for motus */

	(int)opaque;
	cdev->hw->releaseRecordingFrame(mem);
}

static int cam_auto_focus(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->autoFocus();
}

static int cam_cancel_auto_focus(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->cancelAutoFocus();
}

static int cam_take_picture(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->takePicture();
}

static int cam_cancel_picture(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->cancelPicture();
}

static int cam_set_parameters(struct camera_device *device, const char *parms)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);
	String8 str_parms(parms);
	CameraParameters cp(str_parms);

	return cdev->hw->setParameters(cp);
}

static char *cam_get_parameters(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	strcpy(param_str, ((cdev->hw->getParameters()).flatten()).string());
	return param_str;
}

static int cam_send_command(struct camera_device *device, int32_t cmd, int32_t arg1, int32_t arg2)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	return cdev->hw->sendCommand(cmd, arg1, arg2);
}

static int cam_dump(struct camera_device *device, int fd)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);
	const Vector<String16> args;

	return cdev->hw->dump(fd, args);
}

static void cam_release(struct camera_device *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	cdev->hw->release();
}

static int cdev_close(hw_device_t *device)
{
	struct motus_camera_device* cdev =
		reinterpret_cast<struct motus_camera_device*>(device);

	free(cdev->hw);
	free(device);

	return 0;
}

int motus_camera_device_open_cpp(const hw_module_t* module,
                             const char* name,
                             hw_device_t** device) {
    struct motus_camera_device* cdev = NULL;
    int ret = 0;

    if (NULL == device) {
        ret = -EINVAL;
        goto bailout;
    }

    *device = NULL;

    /*if (strcmp(name, "camera") != 0) {
        ret = -EINVAL;
        goto bailout;
    }*/

    cdev = (struct motus_camera_device*)calloc(1,
            sizeof(struct motus_camera_device));

    if (NULL == cdev) {
        ret = -ENOMEM;
        goto bailout;
    }

    cdev->device.common.tag = HARDWARE_DEVICE_TAG;
    cdev->device.common.version = CAMERA_DEVICE_API_VERSION_1_0;
    cdev->device.common.module = (struct hw_module_t *) module;
    cdev->device.common.close = cdev_close;

    cdev->ops.set_preview_window = cam_set_preview_window;
    cdev->ops.set_callbacks = cam_set_callbacks;
    cdev->ops.enable_msg_type = cam_enable_msg_type;
    cdev->ops.disable_msg_type = cam_disable_msg_type;
    cdev->ops.msg_type_enabled = cam_msg_type_enabled;
    cdev->ops.start_preview = cam_start_preview;
    cdev->ops.stop_preview = cam_stop_preview;
    cdev->ops.preview_enabled = cam_preview_enabled;
    cdev->ops.store_meta_data_in_buffers = cam_store_meta_data_in_buffers;
    cdev->ops.start_recording = cam_start_recording;
    cdev->ops.stop_recording = cam_stop_recording;
    cdev->ops.recording_enabled = cam_recording_enabled;
    cdev->ops.release_recording_frame = cam_release_recording_frame;
    cdev->ops.auto_focus = cam_auto_focus;
    cdev->ops.cancel_auto_focus = cam_cancel_auto_focus;
    cdev->ops.take_picture = cam_take_picture;
    cdev->ops.cancel_picture = cam_cancel_picture;
    cdev->ops.set_parameters = cam_set_parameters;
    cdev->ops.get_parameters = cam_get_parameters;
    cdev->ops.send_command = cam_send_command;
    cdev->ops.dump = cam_dump;
    cdev->ops.release = cam_release;

    cdev->hw = new QualcommCameraHardware("main");
    cdev->device.ops = &cdev->ops;
    *device = &cdev->device.common;

bailout:
    if ((0 != ret) && (NULL != cdev)) {
        free(cdev);
    }

    return 0;
}

} //namespace android

extern "C" int motus_camera_device_open(const hw_module_t* module,
                             const char* name,
                             hw_device_t** device) {
	return android::motus_camera_device_open_cpp(module, name, device);
}
