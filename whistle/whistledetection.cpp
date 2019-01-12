#include "whistledetection.h"

#include <algorithm>
#include <algorithm_ext.h>
#include <cmath>

WhistleDetection::WhistleDetection(uint32_t rate, uint32_t frame_size) :
        rate(rate), frame_size(frame_size), spec_size(frame_size/2+1),
        num_features_noise(spec_size/frame_block_size),
        real_buffer(frame_size), complex_buffer(spec_size),
        last_frames(std::vector<float>(num_features_noise, 0.f)),
        accumulated(num_features_noise, 0.f) {
    plan = fftwf_plan_dft_r2c_1d(frame_size, real_buffer.data(), complex_buffer.data(),
                                 FFTW_ESTIMATE);
}

WhistleDetection::~WhistleDetection() {
    fftwf_destroy_plan(plan);
}

bool WhistleDetection::process(const std::vector<int16_t> &vals) {
    htwk::copy(vals, real_buffer.begin());
    fftwf_execute(plan);
    std::vector<float> spec_buffer(spec_size);
    htwk::transform(complex_buffer, spec_buffer.begin(), [](const fftwf_complex& c) {
        return std::isnan(c[0]) ? 0.f : std::abs(c[0]);
    });
    std::vector<float> discretized = discretize(spec_buffer);
    htwk::pointwise_minuseq(accumulated, last_frames.oldest());
    htwk::pointwise_pluseq(accumulated, discretized);
    last_frames.push(discretized);
    float noise = htwk::sum(accumulated);
    noise_sum -= last_noise.oldest();
    noise_sum += noise;
    last_noise.push(noise);
    if (init_frames >= noise_to_aggregate) {
        float noise_avg = noise_sum / noise_to_aggregate / num_features_noise;
        float sum = 0.f;
        for (float f : accumulated) {
            if (f >= noise_avg * 10.f) sum += f;
        }
        float limit = std::max(sum / num_features_noise, noise_avg * 10.f);
        float score = 0.6050399f;
        static constexpr std::array<float,num_features_detection> coefficients
                {-19.1711084f, -19.1711084f,  16.8218568f,  15.6424958f,
                 1.1391718f, -19.1711084f};
        for (int i = 0; i < num_features_detection; i++) {
            score += accumulated[i] >= limit ? coefficients[i] : 0.f;
        }
        return std::exp(score)/(1+std::exp(score)) > 0.9f;
    }
    init_frames++;
    return false;
}

std::vector<float> WhistleDetection::discretize(const std::vector<float>& spec_buffer) {
    std::vector<float> discretized(num_features_noise, 0.f);
    for (size_t i = 0; i < num_features_noise; i++) {
        for (size_t j = i * frame_block_size; j < (i+1) * frame_block_size; j++) {
            discretized[i] += spec_buffer[j];
        }
    }
    return discretized;
}
