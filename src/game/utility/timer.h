#ifndef SNEKON_TIMER_H
#define SNEKON_TIMER_H

#include <cstdint>

class FrameTimer {
public:
    FrameTimer(uint8_t target_frame);

    auto update() -> void;
    auto wait() -> void;

public:
    auto delta() -> double;

private:
    double m_delta = 0.0;
    double m_frame_now = 0.0;
    double m_frame_last = 0.0;

    const double m_frame_time;
};

#endif //SNEKON_TIMER_H
