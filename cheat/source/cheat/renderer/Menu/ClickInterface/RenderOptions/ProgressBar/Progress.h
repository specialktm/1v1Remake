#pragma once
#include "../../imgui/imgui.h"
#include <chrono>
#include <algorithm> 


class ProgressBar {
public:
    explicit ProgressBar(float maxValue = 1.0f, ImVec2 size = ImVec2(200, 20));

    void Update();
    void Render(const char* overlay_text = "");

    void SetValue(float newValue)
	{
        m_targetValue = newValue;
    }

    [[nodiscard]] float GetValue() const
	{
        return m_targetValue;
    }

    void IncreaseValue(float increasedBy)
    {
        float IncrementedValue = m_targetValue + increasedBy;
        m_targetValue = IncrementedValue;
    }
private:
    float m_value;
    float m_targetValue;
    float m_maxValue;
    ImVec2 m_size;
    float m_speed;

    std::chrono::steady_clock::time_point m_lastUpdate;
};
