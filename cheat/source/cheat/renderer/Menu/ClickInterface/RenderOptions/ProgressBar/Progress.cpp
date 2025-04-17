#include "Progress.h"
#include <cmath>
#include "../../imgui/imgui_internal.h"

ProgressBar::ProgressBar(float maxValue, ImVec2 size)
    : m_value(0.0f), m_targetValue(0.0f), m_maxValue(maxValue), m_size(size), m_speed(10.0f), // Increased speed
    m_lastUpdate(std::chrono::steady_clock::now()) {}


void ProgressBar::Update()
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> deltaTime = now - m_lastUpdate;
    m_lastUpdate = now;

    m_value = std::lerp(m_value, m_targetValue, deltaTime.count() * m_speed);

    if (std::abs(m_value - m_targetValue) < 0.01f) {
        m_value = m_targetValue; 
    }
}


void ProgressBar::Render(const char* overlay_text)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 full_size = ImGui::CalcItemSize(m_size, ImGui::CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
    ImRect bb(pos, ImVec2(pos.x + full_size.x, pos.y + full_size.y));
    ImGui::ItemSize(full_size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, 0)) return;

    ImU32 bg_col = ImGui::GetColorU32(ImGuiCol_FrameBg);
    ImU32 fg_col = ImGui::GetColorU32(ImGuiCol_PlotHistogram);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_col, style.FrameRounding);

    float fraction = (m_maxValue > 0.0f) ? (m_value / m_maxValue) : 0.0f;
    if (fraction > 0.0f) { 
        float progress_width = (bb.Max.x - bb.Min.x) * std::clamp(fraction, 0.0f, 1.0f);
        ImRect progress_rect(bb.Min, ImVec2(bb.Min.x + progress_width, bb.Max.y));
        window->DrawList->AddRectFilled(progress_rect.Min, progress_rect.Max, fg_col, style.FrameRounding);
    }

    if (overlay_text) {
        ImVec2 text_size = ImGui::CalcTextSize(overlay_text);
        auto text_pos = ImVec2{
            bb.Min.x + (bb.Max.x - bb.Min.x - text_size.x) * 0.5f,
            bb.Min.y + (bb.Max.y - bb.Min.y - text_size.y) * 0.5f
        };
        ImGui::RenderTextClipped(bb.Min, bb.Max, overlay_text, nullptr, &text_size, ImVec2(0.5f, 0.5f));
    }
}
