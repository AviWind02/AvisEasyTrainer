#pragma once
namespace Render::Draw
{
    void AddText(const std::optional<std::string>& left,
        const std::optional<std::string>& center,
        const std::optional<std::string>& right,
        ImVec2 pos, ImVec2 size,
        ImU32 color,
        ImFont* font);

    void DrawTextLine(ImVec2 base, ImVec2 size, int line,
        const std::string& left, const std::string& center,
        const std::string& right, ImU32 color,
        ImFont* font, float spacing);

    void DrawRect(ImVec2 pos, ImVec2 size, ImU32 color,
        float rounding = 0.f,
        ImDrawFlags flags = ImDrawFlags_RoundCornersAll);

    void DrawLine(ImVec2 from, ImVec2 to, ImU32 color, float thickness);

    void DrawOutlinedRect(ImVec2 pos, ImVec2 size,
        ImU32 fillColor, ImU32 outlineColor,
        float rounding = 0.0f, float thickness = 1.0f);

    void DrawProgressBar(ImVec2 pos, ImVec2 size,
        float value01, ImU32 fillColor, ImU32 bgColor);

    void DrawTextLabel(ImVec2 pos, const std::string& text,
        ImU32 textColor, ImFont* font = nullptr,
        ImU32 bgColor = 0, float padding = 4.0f);

    void DrawSpinner(ImVec2 center, float radius, ImU32 color,
        int segments = 12, float speed = 1.5f);

    void DrawCrosshair(ImVec2 center, float size, ImU32 color,
        float thickness = 1.0f);

    void AddWrappedText(const std::string& text, ImVec2 pos, float wrapWidth, ImU32 color, ImFont* font = nullptr);

}
