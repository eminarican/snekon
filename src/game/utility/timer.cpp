#include "GLFW/glfw3.h"

#include <thread>

#include "timer.h"

FrameTimer::FrameTimer(uint8_t target_frame) : m_frame_time(1.0 / target_frame) {}

auto FrameTimer::update() -> void {
    m_frame_now = glfwGetTime();
    m_delta = m_frame_now - m_frame_last;
    m_frame_last = m_frame_now;
}

auto FrameTimer::wait() -> void {
    double frame_elapsed = glfwGetTime() - m_frame_now;

    if (frame_elapsed < m_frame_time)
    {
        double time_frame_left = m_frame_time - frame_elapsed;

        if (time_frame_left > 0)
            std::this_thread::sleep_for(
                std::chrono::duration<double>(time_frame_left)
            );
    }
}

auto FrameTimer::delta() -> double {
    return m_delta;
}
