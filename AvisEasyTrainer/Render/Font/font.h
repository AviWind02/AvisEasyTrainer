#pragma once
#include <ShlObj.h>
#include "rubikFont.h"
#include "Icons/IconLoader.h"

namespace FontManager {
    enum class FontSize { Small, Medium, Large };

    struct Fonts {
        ImFont* Small = nullptr;
        ImFont* Medium = nullptr;
        ImFont* Large = nullptr;
    };

    inline Fonts Regular;
    inline Fonts Title;

    inline std::filesystem::path GetWindowsFontPath() {
        PWSTR fontDir = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &fontDir))) {
            std::filesystem::path path(fontDir);
            CoTaskMemFree(fontDir);
            return path;
        }
        return {};
    }

    inline void InitializeFonts() {
        ImGuiIO& io = ImGui::GetIO();
        float smallf = 20.f, medium = 28.f, large = 36.f;

        auto fontFolder = GetWindowsFontPath();
        loghandler::sdk->logger->Info(loghandler::handle, "Initializing fonts...");

        std::vector<std::wstring> candidates = {
            L"segoeui.ttf", L"arial.ttf", L"tahoma.ttf", L"msyh.ttf", L"simhei.ttf"
        };

        bool loaded = false;
        for (const auto& fontName : candidates) {
            auto path = fontFolder / fontName;
            if (std::filesystem::exists(path)) {
                std::string pathStr = path.string();
                loghandler::sdk->logger->InfoF(loghandler::handle, "Trying to load font: %s", pathStr.c_str());

                Regular.Small = io.Fonts->AddFontFromFileTTF(pathStr.c_str(), smallf);
                ImGui::MergeIconsWithLatestFont(smallf);
                Regular.Medium = io.Fonts->AddFontFromFileTTF(pathStr.c_str(), medium);
                ImGui::MergeIconsWithLatestFont(medium);
                Regular.Large = io.Fonts->AddFontFromFileTTF(pathStr.c_str(), large);

                loaded = (Regular.Small && Regular.Medium && Regular.Large);
                if (loaded) {
                    loghandler::sdk->logger->Info(loghandler::handle, "Font loaded from file.");
                    break;
                }
                else {
                    loghandler::sdk->logger->WarnF(loghandler::handle, "Font loaded but one or more sizes are null: %s", pathStr.c_str());
                }
            }
        }

        if (!loaded) {
            loghandler::sdk->logger->WarnF(loghandler::handle, "No valid font file found. Falling back to embedded memory font.");

            Regular.Small = io.Fonts->AddFontFromMemoryTTF((void*)rubikFont, rubikFontSize, smallf);
            if (!Regular.Small) {
                Regular.Small = io.Fonts->AddFontDefault();
                loghandler::sdk->logger->WarnF(loghandler::handle, "Embedded Small font failed. Using default.");
            }
            ImGui::MergeIconsWithLatestFont(smallf);

            Regular.Medium = io.Fonts->AddFontFromMemoryTTF((void*)rubikFont, rubikFontSize, medium);
            if (!Regular.Medium)
                Regular.Medium = Regular.Small;

            ImGui::MergeIconsWithLatestFont(medium);

            Regular.Large = io.Fonts->AddFontFromMemoryTTF((void*)rubikFont, rubikFontSize, large);
            if (!Regular.Large)
                Regular.Large = Regular.Medium;

            loghandler::sdk->logger->Info(loghandler::handle, "Embedded font initialized.");
        }

        if (!Regular.Small || !Regular.Medium || !Regular.Large) {
            loghandler::sdk->logger->Error(loghandler::handle, "Font initialization failed. At least one size is null.");
        }

        loghandler::sdk->logger->Info(loghandler::handle, "Font initialization complete.");
    }
}