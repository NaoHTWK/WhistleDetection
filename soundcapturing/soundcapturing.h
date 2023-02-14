#pragma once

#include <string>
class SoundCapturing {
public:
    SoundCapturing() = default;
    void process(std::string device_name);

    enum Microphones { MIC_LEFT_FRONT = 0, MIC_RIGHT_FRONT = 1, MIC_LEFT_BACK = 2, MIC_RIGHT_BACK = 3 };
};
