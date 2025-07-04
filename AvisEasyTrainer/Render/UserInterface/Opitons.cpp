#include "pch.h"
#include "Style.h"
#include "Controls/controls.h"
#include <imgui_internal.h>

using namespace g_feature::g_math;
using namespace Render::Draw;
using namespace Controls;

namespace UI
{
    using namespace Layout;

    bool DrawOptionTextRAW(const std::string& left, const std::string& center, const std::string& right, const std::string& tip)
    {

        bool isBreak = (left.find("#BREAK") != std::string::npos);

        std::string cleanedLeft = left;
        if (isBreak)
            cleanedLeft.erase(cleanedLeft.find("#BREAK"), 6);

        if (isBreak && currentOption == optionIndex + 1) {
            if (Down()) currentOption++;
            else if (Up()) currentOption--;
        }

        optionIndex++;

        const float spacing = Layout::OptionHeight;
        const float padX = Layout::OptionPaddingX;

        int footerHeight = UI::Header::Height;
        int availableHeight = menuBox.size.y - UI::Header::Height - footerHeight;
        int maxVisible = (std::max)(1, int(availableHeight / UI::Layout::OptionHeight));     
        int startOpt = ((currentOption - 1) / maxVisible) * maxVisible + 1;
        int endOpt = (std::min)(startOpt + maxVisible - 1, optionIndex);

        if (optionIndex < startOpt || optionIndex > endOpt)
            return false;

        static float smoothY = menuBox.pos.y + Header::Height;
        ImVec2 optionSize = { menuBox.size.x - 2 * padX, spacing };
        ImVec2 optionPos = {
            menuBox.pos.x + padX,
            menuBox.pos.y + Header::Height + (optionIndex - startOpt) * spacing
        };

        // Smooth highlight bar animation
        if (currentOption == optionIndex)
            smoothY += (optionPos.y - smoothY) * Animation::SmoothSpeed;

        ImVec2 cursor = ImGui::GetIO().MousePos;
        bool hovered = (cursor.x >= optionPos.x && cursor.x <= optionPos.x + optionSize.x &&
            cursor.y >= optionPos.y && cursor.y <= optionPos.y + optionSize.y);
        bool clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

        ImFont* font = FontManager::Regular.Medium;
        float fontHeight = font->FontSize;
        float verticalCenterY = optionPos.y + (spacing - fontHeight) * 0.5f;

        // Highlight bar (active + hovered)
        if (currentOption == optionIndex)
        {
            DrawRect({ optionPos.x, smoothY }, optionSize, isBreak ? Colors::Transparent : Colors::ActiveHighlight, Layout::FrameRounding);
            InfoBox::SetInfoBoxText(tip);
        }

        if (hovered)
            DrawRect(optionPos, optionSize, isBreak ? Colors::Transparent : Colors::HoverBg, Layout::FrameRounding);

        // Horizontal text offset
        float labelOffsetX = Layout::LabelOffsetX; 

        // Dynamically! calculate text baseline
        AddText(
            cleanedLeft.empty() ? std::nullopt : std::optional<std::string>{ left },
            center.empty() ? std::nullopt : std::optional<std::string>{ center },
            right.empty() ? std::nullopt : std::optional<std::string>{ right },
            { optionPos.x + labelOffsetX, verticalCenterY },
            optionSize,
            isBreak ? Colors::MutedText : Colors::Text,
            font
        );


        return ((currentOption == optionIndex || hovered) && (clicked || selectPressed));
    }
    bool DrawIntOption(const std::string& label, int& value, int min, int max, int step, bool useToggle, bool& toggle, const std::string& tip)
    {
        using namespace Layout;

        bool clicked = DrawOptionTextRAW(label, "", "", tip);
        bool selected = (currentOption == optionIndex);
        bool changed = false;


        int footerHeight = UI::Header::Height;
        int availableHeight = menuBox.size.y - UI::Header::Height - footerHeight;
        int maxVisible = (std::max)(1, int(availableHeight / UI::Layout::OptionHeight));     
        int startOpt = ((currentOption - 1) / maxVisible) * maxVisible + 1;
        int endOpt = (std::min)(startOpt + maxVisible - 1, optionIndex);

        if (optionIndex >= startOpt && optionIndex <= endOpt)
        {
            float yBase = menuBox.pos.y + Header::Height + (optionIndex - startOpt) * OptionHeight - OptionPaddingY;
            float sliderWidth = SliderWidth;
            float toggleWidth = useToggle ? Toggle::Size + Padding : 0.f;

            float sliderX = menuBox.pos.x + menuBox.size.x - Padding - sliderWidth;
            float sliderY = yBase + (OptionHeight - Slider::Height) * 0.5f;
            ImVec2 sliderPos(sliderX, sliderY);

            ImGui::SetCursorScreenPos(sliderPos);
            ImGui::PushItemWidth(sliderWidth);

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Slider::Rounding);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Slider::FramePadding);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Slider::ItemSpacing);
            ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, Slider::Rounding);

            ImGui::PushStyleColor(ImGuiCol_FrameBg, Slider::BgColor);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, Slider::GrabColor);
            ImGui::PushStyleColor(ImGuiCol_Border, Colors::Border);

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + OptionPaddingX);
            std::string sliderId = "##Slider_" + label;
            if (ImGui::SliderInt(sliderId.c_str(), &value, min, max)) {
                toggle = true;
                changed = true;
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(4);
            ImGui::PopItemWidth();

            if (useToggle)
            {
                float toggleX = sliderX - toggleWidth - OptionPaddingX;
                float toggleY = sliderY;
                ImVec2 togglePos(toggleX, toggleY);

                ImGui::SetCursorScreenPos(togglePos);
                std::string checkboxId = "##Toggle_" + label;

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Toggle::Rounding);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Toggle::FramePadding);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Toggle::ItemSpacing);

                ImGui::PushStyleColor(ImGuiCol_FrameBg, toggle ? Toggle::OnColor : Toggle::OffColor);
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Toggle::OnColor);
                ImGui::PushStyleColor(ImGuiCol_CheckMark, Colors::Text);

                if (ImGui::Checkbox(checkboxId.c_str(), &toggle)) {
                    changed = true;
                }

                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar(3);
            }

            if (selected)
            {
                if (leftPressed) {
                    value -= step;
                    if (value < min) value = max;
                    toggle = true;
                    changed = true;
                }
                if (rightPressed) {
                    value += step;
                    if (value > max) value = min;
                    toggle = true;
                    changed = true;
                }
                if (clicked) {
                    toggle = !toggle;
                    changed = true;
                }
            }
        }

        return clicked || changed;
    }

    bool DrawToggleOption(const std::string& label, bool& toggle, const std::string& tip)
    {

        bool clicked = DrawOptionTextRAW(label, "", "", tip);
        bool selected = (currentOption == optionIndex);
        bool changed = false;


        int footerHeight = UI::Header::Height;
        int availableHeight = menuBox.size.y - UI::Header::Height - footerHeight;
        int maxVisible = (std::max)(1, int(availableHeight / UI::Layout::OptionHeight));     
        int startOpt = ((currentOption - 1) / maxVisible) * maxVisible + 1;
        int endOpt = (std::min)(startOpt + maxVisible - 1, optionIndex);

        if (optionIndex >= startOpt && optionIndex <= endOpt)
        {
            float yBase = menuBox.pos.y + Header::Height + (optionIndex - startOpt) * OptionHeight - OptionPaddingY;
            float checkBoxSize = Toggle::Size;
            float checkBoxX = menuBox.pos.x + menuBox.size.x - Padding - checkBoxSize;
            float checkBoxY = yBase + (OptionHeight - checkBoxSize) * 0.5f;
            ImVec2 checkBoxPos(checkBoxX, checkBoxY);

            ImGui::SetCursorScreenPos(checkBoxPos);

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Toggle::Rounding);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Toggle::FramePadding);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Toggle::ItemSpacing);

            ImGui::PushStyleColor(ImGuiCol_FrameBg, toggle ? Toggle::OnColor : Toggle::OffColor);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Toggle::OnColor);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, Colors::Text);

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - Toggle::ToggleOffsetX);

            std::string checkboxId = "##Toggle_" + label;
            if (ImGui::Checkbox(checkboxId.c_str(), &toggle))
                changed = true;

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(3);

            if (selected && clicked) {
                toggle = !toggle;
                changed = true;
            }
        }

        return clicked || changed;
    }
    bool DrawFloatOption(const std::string& label, float& value, float min, float max, float step, bool useToggle, bool& toggle, const std::string& tip)
    {
        bool clicked = DrawOptionTextRAW(label, "", "", tip);
        bool selected = (currentOption == optionIndex);
        bool changed = false;

        int footerHeight = UI::Header::Height;
        int availableHeight = menuBox.size.y - UI::Header::Height - footerHeight;
        int maxVisible = (std::max)(1, int(availableHeight / OptionHeight));             int startOpt = ((currentOption - 1) / maxVisible) * maxVisible + 1;
        int endOpt = (std::min)(startOpt + maxVisible - 1, optionIndex);

        if (optionIndex >= startOpt && optionIndex <= endOpt)
        {
            float yBase = menuBox.pos.y + Header::Height + (optionIndex - startOpt) * OptionHeight - OptionPaddingY;
            float sliderWidth = SliderWidth;
            float toggleWidth = useToggle ? Toggle::Size + Padding : 0.f;

            float sliderX = menuBox.pos.x + menuBox.size.x - Padding - sliderWidth;
            float sliderY = yBase + (OptionHeight - Slider::Height) * 0.5f;
            ImVec2 sliderPos(sliderX, sliderY);

            ImGui::SetCursorScreenPos(sliderPos);
            ImGui::PushItemWidth(sliderWidth);

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Slider::Rounding);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Slider::FramePadding);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Slider::ItemSpacing);
            ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, Slider::Rounding);

            ImGui::PushStyleColor(ImGuiCol_FrameBg, Slider::BgColor);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, Slider::GrabColor);
            ImGui::PushStyleColor(ImGuiCol_Border, Colors::Border);

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + OptionPaddingX);
            std::string sliderId = "##FloatSlider_" + label;
            if (ImGui::SliderFloat(sliderId.c_str(), &value, min, max, "%.2f", step)) {
                toggle = true;
                changed = true;
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(4);
            ImGui::PopItemWidth();

            if (useToggle)
            {
                float toggleX = sliderX - toggleWidth - OptionPaddingX;
                float toggleY = sliderY;
                ImVec2 togglePos(toggleX, toggleY);

                ImGui::SetCursorScreenPos(togglePos);
                std::string checkboxId = "##Toggle_" + label;

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Toggle::Rounding);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Toggle::FramePadding);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Toggle::ItemSpacing);

                ImGui::PushStyleColor(ImGuiCol_FrameBg, toggle ? Toggle::OnColor : Toggle::OffColor);
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Toggle::OnColor);
                ImGui::PushStyleColor(ImGuiCol_CheckMark, Colors::Text);

                if (ImGui::Checkbox(checkboxId.c_str(), &toggle)) {
                    changed = true;
                }

                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar(3);
            }

            if (selected)
            {
                if (leftPressed) {
                    value -= step;
                    if (value < min) value = max;
                    toggle = true;
                    changed = true;
                }
                if (rightPressed) {
                    value += step;
                    if (value > max) value = min;
                    toggle = true;
                    changed = true;
                }
                if (clicked) {
                    toggle = !toggle;
                    changed = true;
                }
            }
        }

        return clicked || changed;
    }



}
