/*
 * Copyright (C) 2014 The Android Open Source Project
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

#define LOG_TAG "AudioHAL:audio_hal_thunks"
#include <utils/Log.h>

#include <errno.h>
#include <stdlib.h>

#include <hardware/hardware.h>
#include <hardware/audio.h>

#include "AudioHardware.h"

namespace android_audio_legacy {

struct motus_audio_device {
    struct audio_hw_device device;
    AudioHardware* hw;

    bool sw_mute;
    float muted_volume;
};

struct motus_stream_out {
    struct audio_stream_out stream;
    AudioHardware* hw;
    AudioStreamOut* impl;
};

struct motus_stream_in {
    struct audio_stream_in stream;
    AudioStreamIn* impl;
};

static char param_str[255];

/*******************************************************************************
 *
 * Audio output stream stubs.
 *
 ******************************************************************************/

static int out_set_volume(struct audio_stream_out *stream,
                          float left,
                          float right)
{
    (void) stream;
    (void) left;
    (void) right;
    return 0;
}

static int out_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    (void) stream;
    (void) effect;
    return 0;
}

static int out_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    (void) stream;
    (void) effect;
    return 0;
}

/*******************************************************************************
 *
 * Audio output stream implementation
 *
 ******************************************************************************/

static uint32_t out_get_sample_rate(const struct audio_stream *stream)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    return tstream->impl->sampleRate();
}

static int out_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    if (rate != tstream->impl->sampleRate())
        return -EINVAL;

    return 0;
}

static size_t out_get_buffer_size(const struct audio_stream *stream)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    return tstream->impl->bufferSize();
}

static audio_channel_mask_t out_get_channels(const struct audio_stream *stream)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    return tstream->impl->channels();
}

static audio_format_t out_get_format(const struct audio_stream *stream)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    if (tstream->impl->format() == AudioSystem::PCM_16_BIT)
        return ::AUDIO_FORMAT_PCM_16_BIT;

    return ::AUDIO_FORMAT_PCM_8_BIT;
}

static int out_set_format(struct audio_stream *stream, audio_format_t format)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    if (format == ::AUDIO_FORMAT_PCM_16_BIT &&
        tstream->impl->format() == AudioSystem::PCM_16_BIT)
        return 0;

    /*if (format == ::AUDIO_FORMAT_PCM_8_BIT &&
        tstream->impl->format() == AudioSystem::PCM_8_BIT)
        return 0;*/

    return -EINVAL;
}

static uint32_t out_get_latency(const struct audio_stream_out *stream)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    return tstream->impl->latency();
}

static int out_standby(struct audio_stream *stream)
{
    struct motus_stream_out* tstream =
        reinterpret_cast<struct motus_stream_out*>(stream);

    return tstream->impl->standby();
}

static int out_dump(const struct audio_stream *stream, int fd)
{
    /*const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    const android::Vector<String16> args;

    return tstream->impl->dump(fd, args);*/
    return 0;
}

static int out_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    struct motus_stream_out* tstream =
        reinterpret_cast<struct motus_stream_out*>(stream);
	const android::String8 str_kvpairs(kvpairs);

    return tstream->impl->setParameters(str_kvpairs);
}

static char* out_get_parameters(const struct audio_stream *stream, const char *keys)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);
	const android::String8 str_keys(keys);

    strcpy(param_str, tstream->impl->getParameters(str_keys).string());

    return param_str;
}

static ssize_t out_write(struct audio_stream_out *stream, const void* buffer,
                         size_t bytes)
{
    struct motus_stream_out* tstream =
        reinterpret_cast<struct motus_stream_out*>(stream);

    return tstream->impl->write(buffer, bytes);
}

static int out_get_render_position(const struct audio_stream_out *stream,
                                   uint32_t *dsp_frames)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    return tstream->impl->getRenderPosition(dsp_frames);
}

/*static int out_get_presentation_position(const struct audio_stream_out *stream,
                               uint64_t *frames, struct timespec *timestamp)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);
    uint32_t frm;
    status_t ret;

    return tstream->impl->getPresentationPosition(frames, timestamp);
}*/

static int out_get_next_write_timestamp(const struct audio_stream_out *stream,
                                        int64_t *timestamp)
{
    const struct motus_stream_out* tstream =
        reinterpret_cast<const struct motus_stream_out*>(stream);

    return tstream->impl->getNextWriteTimestamp(timestamp);
}

/*******************************************************************************
 *
 * Audio input stream implementation
 *
 ******************************************************************************/

static uint32_t in_get_sample_rate(const struct audio_stream *stream)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->sampleRate();
}

static int in_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    if (rate != tstream->impl->sampleRate())
        return -EINVAL;

    return 0; /*tstream->impl->setSampleRate(rate);*/
}

static size_t in_get_buffer_size(const struct audio_stream *stream)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->bufferSize();
}

static uint32_t in_get_channels(const struct audio_stream *stream)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->channels();
}

static audio_format_t in_get_format(const struct audio_stream *stream)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    if (tstream->impl->format() == AudioSystem::PCM_16_BIT)
        return ::AUDIO_FORMAT_PCM_16_BIT;

    return ::AUDIO_FORMAT_PCM_8_BIT;
}

static int in_set_format(struct audio_stream *stream, audio_format_t format)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    if (format == ::AUDIO_FORMAT_PCM_16_BIT &&
        tstream->impl->format() == AudioSystem::PCM_16_BIT)
        return 0;

    /*if (format == ::AUDIO_FORMAT_PCM_8_BIT &&
        tstream->impl->format() == AudioSystem::PCM_8_BIT)
        return 0;*/

    return -EINVAL;
}

static int in_standby(struct audio_stream *stream)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->standby();
}

static int in_dump(const struct audio_stream *stream, int fd)
{
    /*const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    const android::Vector<String16> args;

    return tstream->impl->dump(fd, args);*/
    return 0;
}

static int in_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);
	const android::String8 str_kvpairs(kvpairs);

    return tstream->impl->setParameters(str_kvpairs);
}

static char* in_get_parameters(const struct audio_stream *stream,
                               const char *keys)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);
    const android::String8 str_keys(keys);

    strcpy(param_str, tstream->impl->getParameters(str_keys).string());

    return param_str;
}

static int in_set_gain(struct audio_stream_in *stream, float gain)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->setGain(gain);
}

static ssize_t in_read(struct audio_stream_in *stream, void* buffer,
                       size_t bytes)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->read(buffer, bytes);
}

static uint32_t in_get_input_frames_lost(struct audio_stream_in *stream)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->getInputFramesLost();
}

static int in_add_audio_effect(const struct audio_stream *stream,
                               effect_handle_t effect)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->addAudioEffect(effect);
}

static int in_remove_audio_effect(const struct audio_stream *stream,
                                  effect_handle_t effect)
{
    const struct motus_stream_in* tstream =
        reinterpret_cast<const struct motus_stream_in*>(stream);

    return tstream->impl->removeAudioEffect(effect);
}

/*******************************************************************************
 *
 * Audio device stubs
 *
 ******************************************************************************/

static uint32_t adev_num_channels(audio_channel_mask_t channel_mask)
{
	if (channel_mask == 0)
		return 0;

	if ((channel_mask & AUDIO_CHANNEL_OUT_STEREO) == AUDIO_CHANNEL_OUT_STEREO)
		return 2;
	else
		return 1;
}

static int adev_set_parameters(struct audio_hw_device *dev, const char *kvpairs)
{
    const struct motus_audio_device* adev =
        reinterpret_cast<const struct motus_audio_device*>(dev);
    const android::String8 str_kvpairs(kvpairs);

    return adev->hw->setParameters(str_kvpairs);
}

static char * adev_get_parameters(const struct audio_hw_device *dev,
                                  const char *keys)
{
    const struct motus_audio_device* adev =
        reinterpret_cast<const struct motus_audio_device*>(dev);
	const android::String8 str_keys(keys);

    strcpy(param_str, adev->hw->getParameters(str_keys));

    return param_str;
}

static int adev_set_voice_volume(struct audio_hw_device *dev, float volume)
{
    (void) dev;
    (void) volume;
    return 0;
}

static int adev_set_mode(struct audio_hw_device *dev, audio_mode_t mode)
{
    (void) dev;
    (void) mode;
    return 0;
}

static int adev_set_mic_mute(struct audio_hw_device *dev, bool state)
{
    const struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);

    return adev->hw->setMicMute(state);
}

static int adev_get_mic_mute(const struct audio_hw_device *dev, bool *state)
{
    const struct motus_audio_device* adev =
        reinterpret_cast<const struct motus_audio_device*>(dev);

    return adev->hw->getMicMute(state);
}

static size_t adev_get_input_buffer_size(const struct audio_hw_device *dev,
                                         const audio_config *config)
{
    const struct motus_audio_device* adev =
        reinterpret_cast<const struct motus_audio_device*>(dev);

    return adev->hw->getInputBufferSize(config->sample_rate, config->format,
                                        adev_num_channels(config->channel_mask));
}

static int adev_open_input_stream(struct audio_hw_device *dev,
                                  audio_io_handle_t handle,
                                  audio_devices_t devices,
                                  struct audio_config *config,
                                  struct audio_stream_in **stream_in)
{
    (void) handle;

    int ret = 0;
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);
    struct motus_stream_in *in = NULL;
    uint32_t channels;

    *stream_in = NULL;

    in = reinterpret_cast<struct motus_stream_in*>(
            calloc(1, sizeof(struct motus_stream_in)));
    if (!in)  {
        ret = -ENOMEM;
        goto bailout;
    }

    in->stream.common.get_sample_rate = in_get_sample_rate;
    in->stream.common.set_sample_rate = in_set_sample_rate;
    in->stream.common.get_buffer_size = in_get_buffer_size;
    in->stream.common.get_channels = in_get_channels;
    in->stream.common.get_format = in_get_format;
    in->stream.common.set_format = in_set_format;
    in->stream.common.standby = in_standby;
    in->stream.common.dump = in_dump;
    in->stream.common.set_parameters = in_set_parameters;
    in->stream.common.get_parameters = in_get_parameters;
    in->stream.common.add_audio_effect = in_add_audio_effect;
    in->stream.common.remove_audio_effect = in_remove_audio_effect;
    in->stream.set_gain = in_set_gain;
    in->stream.read = in_read;
    in->stream.get_input_frames_lost = in_get_input_frames_lost;

    channels = adev_num_channels(config->channel_mask);
    in->impl = adev->hw->openInputStream(devices,
                                            reinterpret_cast<int*>(&config->format),
                                            &channels,
                                            &config->sample_rate,
                                            reinterpret_cast<status_t*>(&ret),
                                            (AudioSystem::audio_in_acoustics)0);

    if (NULL == in->impl) {
        ret = -ENOMEM;
        goto bailout;
    }

    if (0 == ret)
        *stream_in = &in->stream;

bailout:
    if ((0 != ret) && (NULL != in)) {
        delete in->impl;
        free(in);
    }

    return ret;
}

static void adev_close_input_stream(struct audio_hw_device *dev,
                                    struct audio_stream_in *stream)
{
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);
    struct motus_stream_in* tstream =
        reinterpret_cast<struct motus_stream_in*>(stream);

    adev->hw->closeInputStream(tstream->impl);
    free(stream);
}

static int adev_open_output_stream(struct audio_hw_device *dev,
                                   audio_io_handle_t handle,
                                   audio_devices_t devices,
                                   audio_output_flags_t flags,
                                   struct audio_config *config,
                                   struct audio_stream_out **stream_out)
{
    (void) handle;
    int ret = 0;
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);
    struct motus_stream_out *out = NULL;
    uint32_t channels;

    *stream_out = NULL;

    out = reinterpret_cast<struct motus_stream_out*>(
            calloc(1, sizeof(struct motus_stream_out)));
    if (!out)  {
        ret = -ENOMEM;
        goto bailout;
    }

    out->stream.common.get_sample_rate = out_get_sample_rate;
    out->stream.common.set_sample_rate = out_set_sample_rate;
    out->stream.common.get_buffer_size = out_get_buffer_size;
    out->stream.common.get_channels = out_get_channels;
    out->stream.common.get_format = out_get_format;
    out->stream.common.set_format = out_set_format;
    out->stream.common.standby = out_standby;
    out->stream.common.dump = out_dump;
    out->stream.common.set_parameters = out_set_parameters;
    out->stream.common.get_parameters = out_get_parameters;
    out->stream.common.add_audio_effect = out_add_audio_effect;
    out->stream.common.remove_audio_effect = out_remove_audio_effect;
    out->stream.get_latency = out_get_latency;
    out->stream.set_volume = out_set_volume;
    out->stream.write = out_write;
    out->stream.get_render_position = out_get_render_position;
    out->stream.get_next_write_timestamp = out_get_next_write_timestamp;
    /*out->stream.get_presentation_position = out_get_presentation_position;*/

    channels = adev_num_channels(config->channel_mask);
    out->impl = adev->hw->openOutputStream(
            reinterpret_cast<uint32_t>(devices),
            reinterpret_cast<int*>(&config->format),
            &channels,
            &config->sample_rate,
            reinterpret_cast<status_t*>(&ret));

    if (NULL == out->impl) {
        ret = -ENOMEM;
        goto bailout;
    }

    if (0 == ret) {
        out->hw = adev->hw;
        *stream_out = &out->stream;
    }

bailout:
    if ((0 != ret) && (NULL != out)) {
        delete out->impl;
        free(out);
    }

    return ret;
}

static void adev_close_output_stream(struct audio_hw_device *dev,
                                     struct audio_stream_out *stream)
{
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);
    struct motus_stream_out* tstream =
        reinterpret_cast<struct motus_stream_out*>(stream);

    adev->hw->closeOutputStream(tstream->impl);
    free(stream);
}

static int adev_init_check(const struct audio_hw_device *dev)
{
    const struct motus_audio_device* adev =
        reinterpret_cast<const struct motus_audio_device*>(dev);

    return adev->hw->initCheck();
}

static int adev_set_master_volume(struct audio_hw_device *dev, float volume)
{
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);

	if (adev->sw_mute) {
		adev->muted_volume = volume;
		return 0;
	}

    return adev->hw->setMasterVolume(volume);
}

static int adev_get_master_volume(struct audio_hw_device *dev,
                                  float *volume)
{
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);

	if (adev->sw_mute) {
		*volume = adev->muted_volume;
		return 0;
	}

    return adev->hw->getMasterVolume(volume);
}

static int adev_set_master_mute(struct audio_hw_device *dev, bool muted)
{
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);

	if (muted) {
		adev->hw->getMasterVolume(&adev->muted_volume);
		adev->sw_mute = true;
	} else {
		adev->hw->setMasterVolume(adev->muted_volume);
		adev->sw_mute = false;
	}

    return 0;
}

static int adev_get_master_mute(struct audio_hw_device *dev,
                                bool *muted)
{
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(dev);

    *muted = adev->sw_mute;

    return 0;
}

static int adev_dump(const audio_hw_device* dev, int fd)
{
    /*const struct motus_audio_device* adev =
        reinterpret_cast<const struct motus_audio_device*>(dev);

    const android::Vector<String16> args;

    return adev->hw->dump(fd, args);*/
    return 0;
}

static int adev_close(hw_device_t *device)
{
    struct motus_audio_device* adev =
        reinterpret_cast<struct motus_audio_device*>(device);

	free(adev->hw);
    free(device);

    return 0;
}

static int motus_audiodev_open_cpp(
        const hw_module_t* module,
        const char* name,
        hw_device_t** device)
{
    struct motus_audio_device* adev = NULL;
    int ret = 0;

    if (NULL == device) {
        ret = -EINVAL;
        goto bailout;
    }

    *device = NULL;

    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0) {
        ret = -EINVAL;
        goto bailout;
    }

    adev = (struct motus_audio_device*)calloc(1,
            sizeof(struct motus_audio_device));
    if (NULL == adev) {
        ret = -ENOMEM;
        goto bailout;
    }
	adev->sw_mute = false;

    adev->device.common.tag = HARDWARE_DEVICE_TAG;
    adev->device.common.version = AUDIO_DEVICE_API_VERSION_2_0;
    adev->device.common.module = (struct hw_module_t *) module;
    adev->device.common.close = adev_close;

    adev->device.init_check = adev_init_check;
    adev->device.set_voice_volume = adev_set_voice_volume;
    adev->device.set_master_volume = adev_set_master_volume;
    adev->device.get_master_volume = adev_get_master_volume;
    adev->device.set_master_mute = adev_set_master_mute;
    adev->device.get_master_mute = adev_get_master_mute;
    adev->device.set_mode = adev_set_mode;
    adev->device.set_mic_mute = adev_set_mic_mute;
    adev->device.get_mic_mute = adev_get_mic_mute;
    adev->device.set_parameters = adev_set_parameters;
    adev->device.get_parameters = adev_get_parameters;
    adev->device.get_input_buffer_size = adev_get_input_buffer_size;
    adev->device.open_output_stream = adev_open_output_stream;
    adev->device.close_output_stream = adev_close_output_stream;
    adev->device.open_input_stream = adev_open_input_stream;
    adev->device.close_input_stream = adev_close_input_stream;
    adev->device.dump = adev_dump;

    adev->hw = new (AudioHardware);
    *device = &adev->device.common;

bailout:
    if ((0 != ret) && (NULL != adev)) {
        free(adev);
    }

    return 0;
}

}  // namespace android_audio_legacy

extern "C" int motus_audiodev_open(const hw_module_t* module,
                             const char* name,
                             hw_device_t** device) {
    return android_audio_legacy::motus_audiodev_open_cpp(module, name, device);
}
