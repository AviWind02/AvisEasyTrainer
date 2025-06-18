// DrawHelpers.cpp
#include "pch.h"
#include "imgui_internal.h"


namespace render::draw
{
    void AddText(const std::optional<std::string>& left,
        const std::optional<std::string>& center,
        const std::optional<std::string>& right,
        ImVec2 pos, ImVec2 size,
        ImU32 color,
        ImFont* font)
    {
     
        auto* drawList = ImGui::GetWindowDrawList();
        if (font) ImGui::PushFont(font);

        if (left)
            drawList->AddText({ pos.x, pos.y }, color, left->c_str());

        if (center)
        {
            float textW = ImGui::CalcTextSize(center->c_str()).x;
            drawList->AddText({ pos.x + (size.x - textW) * 0.5f, pos.y }, color, center->c_str());
        }

        if (right)
        {
            float textW = ImGui::CalcTextSize(right->c_str()).x;
            drawList->AddText({ pos.x + (size.x - textW) - 14.f, pos.y }, color, right->c_str());
        }

        if (font) ImGui::PopFont();
    }

    void AddWrappedText(const std::string& text, ImVec2 pos, float width,
        ImU32 color, ImFont* font) {
        if (!font) font = ImGui::GetFont();
        ImGui::GetWindowDrawList()
            ->AddText(font, ImGui::GetFontSize(), pos, color, text.c_str(), nullptr, width);
    }

    void DrawTextLine(ImVec2 base, ImVec2 size, int line,
        const std::string& left, const std::string& center,
        const std::string& right, ImU32 color,
        ImFont* font, float spacing)
    {
        ImVec2 pos{ base.x, base.y + (line - 1) * spacing };
        AddText(left, center, right, pos, size, color, font);
    }

    void DrawRect(ImVec2 pos, ImVec2 size, ImU32 color,
        float rounding,
        ImDrawFlags flags)
    {
        ImGui::GetWindowDrawList()->AddRectFilled(
            pos,
            { pos.x + size.x, pos.y + size.y },
            color, rounding, flags
        );
    }

    void DrawLine(ImVec2 from, ImVec2 to, ImU32 color, float thickness)
    {
        ImGui::GetWindowDrawList()->AddLine(from, to, color, thickness);
    }

    void DrawOutlinedRect(ImVec2 pos, ImVec2 size, ImU32 fillColor, ImU32 outlineColor, float rounding, float thickness)
    {
        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), fillColor, rounding);
        drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), outlineColor, rounding, 0, thickness);
    }

    void DrawProgressBar(ImVec2 pos, ImVec2 size, float value01, ImU32 fillColor, ImU32 bgColor)
    {
        value01 = std::clamp(value01, 0.0f, 1.0f);
        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor);
        drawList->AddRectFilled(pos, ImVec2(pos.x + size.x * value01, pos.y + size.y), fillColor);
    }

    void DrawTextLabel(ImVec2 pos, const std::string& text, ImU32 textColor, ImFont* font, ImU32 bgColor, float padding)
    {
        auto* drawList = ImGui::GetWindowDrawList();
        if (font) ImGui::PushFont(font);
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        if (bgColor)
        {
            drawList->AddRectFilled(
                ImVec2(pos.x - padding, pos.y - padding),
                ImVec2(pos.x + textSize.x + padding, pos.y + textSize.y + padding),
                bgColor
            );
        }
        drawList->AddText(pos, textColor, text.c_str());
        if (font) ImGui::PopFont();
    }

    void DrawSpinner(ImVec2 center, float radius, ImU32 color, int segments, float speed)
    {
        auto* drawList = ImGui::GetWindowDrawList();
        float time = ImGui::GetTime();
        float startAngle = fmodf(time * speed * 2 * IM_PI, 2 * IM_PI);
        float step = 2 * IM_PI / segments;
        for (int i = 0; i < segments; ++i)
        {
            float angle = startAngle + i * step;
            float alpha = i / (float)segments;
            ImVec2 pos{
                center.x + cosf(angle) * radius,
                center.y + sinf(angle) * radius
            };
            drawList->AddCircleFilled(pos, 2.0f, ImColor(ImVec4(1.0f, 1.0f, 1.0f, alpha)) & color);
        }
    }

    void DrawCrosshair(ImVec2 center, float size, ImU32 color, float thickness)
    {
        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddLine(ImVec2(center.x - size, center.y), ImVec2(center.x + size, center.y), color, thickness);
        drawList->AddLine(ImVec2(center.x, center.y - size), ImVec2(center.x, center.y + size), color, thickness);
    }
}
