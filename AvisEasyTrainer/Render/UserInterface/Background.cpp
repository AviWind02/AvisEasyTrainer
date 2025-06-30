#include "pch.h"
#include "Style.h"
#include "Controls/controls.h"

using namespace g_feature::g_math;
using namespace Render::Draw;

namespace UI
{
    MenuBox menuBox;

    namespace background {

        void DrawTitleBar() {

            std::string title = GetBreadcrumbTitle();
            ImVec2 textPos = { menuBox.pos.x + Layout::OptionPaddingX, menuBox.pos.y + 6.f };
            ImVec2 areaSize = { menuBox.size.x, 50.f };

            AddText(title, "", "", textPos, areaSize, Colors::Text, FontManager::Regular.Medium);
        }

        void DrawFooter() {

            float spacing = Layout::OptionHeight;
            int maxVisible = (std::max)(1, int((menuBox.size.y - Header::Height) / spacing));
            int totalOptions = Controls::optionIndex;
            int totalPages = (totalOptions + maxVisible - 1) / maxVisible;
            int currentPage = (Controls::currentOption - 1) / maxVisible + 1;

            std::string right1 = " | Pg: " + std::to_string(currentPage) + "/" + std::to_string(totalPages);
            std::string right = "Opt: " + std::to_string(Controls::currentOption) + right1;
            std::string left = "EasyTrainer | vPre-Alpha";

            ImVec2 footerPos = { menuBox.pos.x, menuBox.pos.y + menuBox.size.y - Header::Height };
            ImVec2 footerSize = { menuBox.size.x, Header::Height };

            DrawLine(
                { footerPos.x, footerPos.y },
                { footerPos.x + footerSize.x, footerPos.y },
                Colors::Border, 1.0f
            );

            AddText(
                left, "", right,
                { footerPos.x + 10.f, footerPos.y + 16.f },
                footerSize,
                Colors::MutedText,
                FontManager::Regular.Small
            );
        }

        void HandleWindowDraggingAndResizing()
        {
            static bool dragging = false;
            static bool resizing = false;
            static ImVec2 dragOffset{};
            static const float resizeHandleSize = 12.0f;
            static const float minWidth = 100.0f;
            static const float minHeight = 80.0f;

            static ImVec2 targetPos = menuBox.pos;
            static ImVec2 targetSize = menuBox.size;

            ImVec2 cursor = ImGui::GetIO().MousePos;
            ImVec2 windowMin = menuBox.pos;
            ImVec2 windowMax = menuBox.pos + menuBox.size;
            ImVec2 resizeHandleMin = windowMax - ImVec2(resizeHandleSize, resizeHandleSize);
            ImVec2 dragAreaMin = menuBox.pos;
            ImVec2 dragAreaMax = menuBox.pos + ImVec2(menuBox.size.x, Header::Height);

            if (!dragging) {
                if (!resizing && ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                    cursor.x >= resizeHandleMin.x && cursor.x <= windowMax.x &&
                    cursor.y >= resizeHandleMin.y && cursor.y <= windowMax.y)
                {
                    resizing = true;
                }

                if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    resizing = false;
                }

                if (resizing) {
                    ImVec2 newSize = cursor - menuBox.pos;
                    newSize.x = (std::max)(newSize.x, minWidth);
                    newSize.y = (std::max)(newSize.y, minHeight);
                    targetSize = newSize;
                }
            }

            if (!resizing) {
                if (!dragging && ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                    cursor.x >= dragAreaMin.x && cursor.x <= dragAreaMax.x &&
                    cursor.y >= dragAreaMin.y && cursor.y <= dragAreaMax.y)
                {
                    dragging = true;
                    dragOffset = cursor - menuBox.pos;
                }

                if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    dragging = false;
                }

                if (dragging) {
                    targetPos = cursor - dragOffset;
                }
            }

            float speed = Animation::SmoothSpeed;
            menuBox.pos.x += (targetPos.x - menuBox.pos.x) * speed;
            menuBox.pos.y += (targetPos.y - menuBox.pos.y) * speed;
            menuBox.size.x += (targetSize.x - menuBox.size.x) * speed;
            menuBox.size.y += (targetSize.y - menuBox.size.y) * speed;
        }


        void DrawResizeHandleDot()
        {
            static const float dotRadius = 5.0f;
            static const float handleSize = 12.0f;

            ImVec2 windowMax = menuBox.pos + menuBox.size;
            ImVec2 handleMin = windowMax - ImVec2(handleSize, handleSize);
            ImVec2 cursor = ImGui::GetIO().MousePos;

            bool hovering = (cursor.x >= handleMin.x && cursor.x <= windowMax.x &&
                cursor.y >= handleMin.y && cursor.y <= windowMax.y);

            static float highlightAlpha = 0.0f;
            float targetAlpha = hovering ? 1.0f : 0.0f;
            highlightAlpha += (targetAlpha - highlightAlpha) * 0.15f; 

            ImU32 baseColor = Colors::Highlight;
            float r = ((baseColor >> IM_COL32_R_SHIFT) & 0xFF) / 255.0f;
            float g = ((baseColor >> IM_COL32_G_SHIFT) & 0xFF) / 255.0f;
            float b = ((baseColor >> IM_COL32_B_SHIFT) & 0xFF) / 255.0f;

            ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, highlightAlpha));

            ImGui::GetWindowDrawList()->AddCircleFilled(
                windowMax - ImVec2(dotRadius + 2, dotRadius + 2),
                dotRadius,
                color
            );
        }


        void DrawBackgroundWindow() {
            DrawRect(menuBox.pos, menuBox.size, UI::Colors::Background, UI::Layout::FrameRounding);
            HandleWindowDraggingAndResizing();
            DrawResizeHandleDot();
        }
    }
}