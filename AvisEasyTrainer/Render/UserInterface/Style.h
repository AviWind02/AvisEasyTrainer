#pragma once
namespace UI
{
    namespace Colors {
        // Text
        inline ImU32 Text = IM_COL32(255, 255, 255, 255);             // Pure white for high contrast
        inline ImU32 MutedText = IM_COL32(120, 140, 160, 255);        // Desaturated blue-gray

        // Surfaces
        inline ImU32 Background = IM_COL32(8, 10, 18, 255);           // Deep blue-black
        inline ImU32 FrameBg = IM_COL32(18, 22, 34, 255);             // Slightly brighter blue-gray

        // Borders & UI elements
        inline ImU32 Border = IM_COL32(80, 90, 110, 255);             // Muted slate
        inline ImU32 Highlight = IM_COL32(0, 255, 180, 255);          // Bright neon aqua
        inline ImU32 ActiveHighlight = IM_COL32(0, 255, 180, 90);     // Same hue, translucent
        inline ImU32 HoverBg = IM_COL32(30, 60, 90, 160);             // Soft cyan highlight
        inline ImU32 Active = IM_COL32(255, 20, 147, 255);            // Cyberpunk pink
        inline ImU32 Grab = IM_COL32(0, 255, 255, 255);               // Neon teal

        // Utility
        inline ImU32 Transparent = IM_COL32(0, 0, 0, 0);
    }

    namespace Layout {
        inline float Padding = 14.0f;
        inline float FrameRounding = 6.0f;
        inline float FrameHeight = 22.0f;
        inline float OptionHeight = 28.0f;
        inline float OptionPaddingX = 3.0f;
        inline float OptionPaddingY = 5.0f;
        inline float CheckboxSize = 18.0f;
        inline float SliderWidth = 100.0f;
        inline float LabelOffsetX = 8.0f;

        inline ImVec2 ItemSpacing = ImVec2(8.0f, 2.0f);
        inline ImVec2 FramePadding = ImVec2(4.f, 0.5f);
    }

    namespace Toggle {
        inline ImU32 OnColor = Colors::Highlight;
        inline ImU32 OffColor = IM_COL32(40, 40, 60, 255);
        inline float Size = 10.0f;
        inline float Rounding = 4.0f;

        inline ImVec2 FramePadding = Layout::FramePadding;
        inline ImVec2 ItemSpacing = Layout::ItemSpacing;
        inline float BorderSize = 1.0f;
        inline float ToggleOffsetX = 8.0f;
    }

    namespace Slider {
        inline ImU32 BgColor = Colors::FrameBg;
        inline ImU32 GrabColor = Colors::Grab;
        inline float Height = 10.0f; 
        inline float Rounding = 6.0f;

        inline ImVec2 FramePadding = Layout::FramePadding;
        inline ImVec2 ItemSpacing = Layout::ItemSpacing;     
        inline float BorderSize = 1.0f; 
    }

    namespace Input {
        inline ImU32 Bg = IM_COL32(20, 20, 28, 255);
        inline ImU32 Text = Colors::Text;
        inline ImU32 Placeholder = IM_COL32(160, 160, 160, 120);
        inline float Height = 20.0f;
        inline float Rounding = Layout::FrameRounding;
    }

    namespace Header {
        inline ImU32 TitleBar = IM_COL32(20, 22, 28, 255);
        inline ImU32 Footer = IM_COL32(25, 25, 38, 255);
        inline float Height = 40.0f;
    }

    namespace Scroll {
        inline int MaxVisibleOptions = 10;
        inline float ScrollSpeed = 3.0f;
    }

    namespace Animation {
        inline float SmoothSpeed = 0.12f; 
    }
}
