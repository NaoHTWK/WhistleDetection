#include <soundcapturing.h>

#include <whistledetection.h>
#include <fstream>
#include <alsa/asoundlib.h>
#include <ctime>
#include <fftw3.h>
#include <queue>
#include <numeric>
#include <thread>

#define ALSA_PCM_NEW_HW_PARAMS_API

static const int numChannels = 1;

void SoundCapturing::run() {
    int rc;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;

    /* Open PCM device for recording (capture). */
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
        return;
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    snd_pcm_hw_params_set_channels(handle, params, numChannels);

    unsigned  int rate = WhistleDetection::reqRate;
    unsigned long frameSize = WhistleDetection::reqFrameSize;
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, &dir);
    snd_pcm_hw_params_set_period_size_near(handle, params, &frameSize, &dir);
    WhistleDetection whistle_detection(rate, frameSize);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
        return;
    }

    /* Use a buffer large enough to hold one period */
    snd_pcm_uframes_t frames;
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);

    std::vector<short> buffer(frames);

    /* We want to loop for 5 seconds */
    snd_pcm_hw_params_get_period_time(params, &val, &dir);

    while (true) {
        rc = snd_pcm_readi(handle, buffer.data(), frames);
        if (rc == -EPIPE) {
            /* EPIPE means overrun */
            fprintf(stderr, "overrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr, "short read, read %d frames\n", rc);
        }

        if (whistle_detection.process(buffer)) {
            //TODO: Send a signal to the rest of your software to inform it that a whistle was detected.
        }
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
}
void SoundCapturing::startThread() {
    std::thread t(SoundCapturing::run);
    t.detach();
}

