#include "pch.h"
#include "Controls/controls.h"



namespace render::ui
{

    bool ContainsIcon(const std::string& text) {
        for (size_t i = 0; i < text.size(); i++) {
            unsigned char c = text[i];
            if (c >= 0xe0 && c <= 0xf8) {
                if (i + 2 < text.size() && (unsigned char)text[i + 1] >= 0x80 && (unsigned char)text[i + 2] >= 0x80) {
                    return true;
                }
            }
        }
        return false;
    }

    ImVec2 GetParentWindowSize() {
        RECT rect;
        HWND window = GetActiveWindow();
        if (window && GetClientRect(window, &rect))
            return { (float)(rect.right - rect.left), (float)(rect.bottom - rect.top) };
        return { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };
    }

}
