#pragma once


namespace render::ui {

    namespace InfoBox {

        inline std::string currentText = "";

        inline void SetInfoBoxText(const std::string& text) {
            if (text != currentText) {
                currentText = text;
            }
        }

        inline void Render() {

            using namespace UI;
            using namespace render::draw;



            if (!controls::menuOpen)
                return;


            static std::string lastText = "";
            static std::string animatedText = "";
            static size_t     charIndex = 0;
            static float      animStartTime = 0.0f;
            constexpr float   charsPerSecond = 100.0f;

            float now = ImGui::GetTime();

            if (currentText != lastText) {
                lastText = currentText;
                animatedText.clear();
                charIndex = 0;
                animStartTime = now;
            }

            size_t targetChars = static_cast<size_t>((now - animStartTime) * charsPerSecond);
            if (targetChars > charIndex) {
                charIndex = (std::min)(targetChars, currentText.size());
                animatedText = currentText.substr(0, charIndex);
            }

            const float pad = Layout::Padding;
            const float spacing = 15.0f;
            const ImVec2 screenSize = render::ui::GetParentWindowSize();
            const float boxWidth = menuBox.size.x;

            ImVec2 wrappedSize = ImGui::CalcTextSize(animatedText.c_str(), nullptr, true, boxWidth - 2 * pad);
            float boxHeight = wrappedSize.y + 2 * pad;

            ImVec2 belowPos = ImVec2(menuBox.pos.x, menuBox.pos.y + menuBox.size.y + spacing);
            ImVec2 abovePos = ImVec2(menuBox.pos.x, menuBox.pos.y - boxHeight - spacing);
            ImVec2 finalPos = (belowPos.y + boxHeight > screenSize.y) ? abovePos : belowPos;

            ImVec2 boxSize = { boxWidth, boxHeight };

            DrawRect(finalPos, boxSize, Colors::FrameBg, Layout::FrameRounding);

            ImVec2 textPos = { finalPos.x + pad, finalPos.y + pad };
            ImGui::PushTextWrapPos(finalPos.x + boxWidth - pad);
            AddWrappedText(animatedText, textPos, boxWidth - 2 * pad, Colors::Text);
            ImGui::PopTextWrapPos();
        }
    }
}
