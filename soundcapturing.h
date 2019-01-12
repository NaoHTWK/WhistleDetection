#pragma once

class SoundCapturing {
private:
    static void run();

public:
    SoundCapturing() = delete;
    virtual ~SoundCapturing() = delete;

    static void startThread();
};
