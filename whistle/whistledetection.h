#pragma once

#include <vector>

#include <fftw3.h>
#include <fftw_allocator.h>
#include <ring_buffer.h>

class WhistleDetection {
  public:
    static constexpr uint32_t reqRate = 44100;
    static constexpr uint32_t reqFrameSize = 441;

    WhistleDetection(uint32_t rate, uint32_t frame_size);
    ~WhistleDetection();

    bool process(const std::vector<int16_t>& vals);

  private:
    static constexpr int32_t frame_block_size = 10;
    static constexpr int32_t frames_to_aggregate = 60;
    static constexpr int32_t noise_to_aggregate = 1000;
    static constexpr int32_t num_features_detection = 6;

    std::vector<float> discretize(const std::vector<float> &spec_buffer);

    const uint32_t rate;
    const uint32_t frame_size;
    const uint32_t spec_size;
    const size_t num_features_noise;
    fftwf_vector<float> real_buffer;
    fftwf_vector<fftwf_complex> complex_buffer;
    fftwf_plan plan;
    RingBuffer<std::vector<float>, frames_to_aggregate> last_frames;
    std::vector<float> accumulated;
    float noise_sum = 0.f;
    RingBuffer<float, noise_to_aggregate> last_noise{0.f};
    int64_t init_frames = 0;
};
