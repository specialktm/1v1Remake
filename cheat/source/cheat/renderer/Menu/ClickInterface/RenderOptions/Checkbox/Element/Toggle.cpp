#define IMGUI_DEFINE_MATH_OPERATORS
#include "Toggle.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_internal.h"
#include <cmath>
#include <unordered_map> 

namespace Elements
{
    bool Toggle(const char* label, const char* desc ,bool* v)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        ImVec2 pos = window->DC.CursorPos;
        ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        float box_size = ImGui::GetFrameHeight();
        ImRect check_bb(pos, ImVec2(pos.x + box_size, pos.y + box_size));

        ImGui::ItemSize(check_bb, style.FramePadding.y);
        ImRect total_bb(pos, ImVec2(pos.x + box_size + style.ItemInnerSpacing.x + label_size.x, pos.y + box_size));

        if (!ImGui::ItemAdd(total_bb, ImGui::GetID(label)))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, ImGui::GetID(label), &hovered, &held);

        static std::unordered_map<ImGuiID, float> anim_progress_map;
        ImGuiID id = ImGui::GetID(label);
        float& anim_progress = anim_progress_map[id];

        if (pressed)
        {
            *v = !(*v);  
            ImGui::MarkItemEdited(id);
        }


        float target = *v ? 1.0f : 0.0f;
        anim_progress = ImLerp(anim_progress, target, ImGui::GetIO().DeltaTime * 10.0f);

        if (anim_progress < 0.02f)
            anim_progress = 0.0f;

        ImU32 col_bg = ImGui::GetColorU32(ImGuiCol_FrameBg);
        ImU32 col_check = ImGui::GetColorU32(ImGuiCol_CheckMark);
        ImGui::RenderFrame(check_bb.Min, check_bb.Max, col_bg, true, style.FrameRounding);
        ImGui::SameLine(0, style.ItemInnerSpacing.x);

        if (*v || hovered && anim_progress <= 0.0f)
        {
            float time = ImGui::GetTime();
            float pulse = (std::sin(time * 3.0f) * 0.5f) + 0.5f;

            ImVec4 baseColor = ImGui::ColorConvertU32ToFloat4(col_check); 
            baseColor.w = 0.8f + 0.2f * pulse; 

            ImVec4 textGlow = ImVec4(
                baseColor.x + 0.1f * pulse,
                baseColor.y + 0.1f * pulse,
                baseColor.z + 0.2f * pulse,
                baseColor.w
            );

            ImU32 final_glow = ImGui::ColorConvertFloat4ToU32(baseColor);
            ImVec2 glow_expand = ImVec2(2.0f, 2.0f) + ImVec2(1.0f * pulse, 1.0f * pulse);
            ImRect glow_bb = ImRect(check_bb.Min - glow_expand, check_bb.Max + glow_expand);
            window->DrawList->AddRect(glow_bb.Min, glow_bb.Max, final_glow, style.FrameRounding + 2.0f, 0, 2.0f * pulse);
           
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32(textGlow));
            ImGui::TextUnformatted(label);
            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::TextUnformatted(label);
        }

        if (anim_progress > 0.05f)
        {
            float fill_size = anim_progress * box_size;
            ImVec2 fill_center = (check_bb.Min + check_bb.Max) * 0.5f;
            ImVec2 fill_min = ImVec2(fill_center.x - fill_size * 0.5f, fill_center.y - fill_size * 0.5f);
            ImVec2 fill_max = ImVec2(fill_center.x + fill_size * 0.5f, fill_center.y + fill_size * 0.5f);
            window->DrawList->AddRectFilled(fill_min, fill_max, col_check, style.FrameRounding);
        }

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        return pressed;
    }
}
