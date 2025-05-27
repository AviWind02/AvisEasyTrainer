#include "pch.h"
#include "Controls/controls.h"
#include "Render/Hooks/d3d12globals.h"
#include "Render/UserInterface/Style.h"

#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/Entity.hpp>
#include <RED4ext/Scripting/Natives/Generated/WorldTransform.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/IPlacedComponent.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatType.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatsSystem.hpp>

#include <RED4ext/Scripting/Natives/Generated/game/StatPoolsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatPoolType.hpp>

namespace render::ui
{

    RED4ext::ent::Entity* GetPlayerEntity()
    {
        using namespace RED4ext;

        ScriptGameInstance gameInstance;
        Handle<IScriptable> handle;
        if (!ExecuteGlobalFunction("GetPlayer;GameInstance", &handle, gameInstance) || !handle)
            return nullptr;

        return reinterpret_cast<ent::Entity*>(handle.instance);
    }

    bool GetPlayerHandleAndID(RED4ext::Handle<RED4ext::IScriptable>& outHandle, RED4ext::ent::EntityID& outID)
    {
        using namespace RED4ext;

        ScriptGameInstance gameInstance;
        if (!ExecuteGlobalFunction("GetGameInstance", &gameInstance))
            return false;

        if (!ExecuteGlobalFunction("GetPlayer;GameInstance", &outHandle, gameInstance) || !outHandle)
            return false;

        auto* rtti = CRTTISystem::Get();
        auto* getEntityID = rtti->GetClass("entEntity")->GetFunction("GetEntityID");
        StackArgs_t args;

        return ExecuteFunction(outHandle, getEntityID, &outID, args) && outID.hash != 0;
    }

        // NOTE: StatPoolType values are treated as percentages (0–100%). 
        // Any value >100 will be automatically clamped to 100%. 
        // Keep your sliders within 0–100 (or use a 0.0–1.0 slider and multiply by 100) 
        //  to avoid unexpected maxing out.


    float GetStatValue(RED4ext::game::data::StatPoolType poolType)
    {
        using namespace RED4ext;

        // 1) grab the player
        Handle<IScriptable> playerHandle;
        ent::EntityID playerID;
        if (!GetPlayerHandleAndID(playerHandle, playerID))
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[GetStatValue(pool)] failed to get player handle/ID");
            return -1.0f;
        }

        // 2) get the game instance
        ScriptGameInstance gameInstance;
        if (!ExecuteGlobalFunction("GetGameInstance", &gameInstance))
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[GetStatValue(pool)] GetGameInstance failed");
            return -1.0f;
        }

        // 3) fetch the StatPoolsSystem
        Handle<game::StatPoolsSystem> statPoolsSystem;
        if (!ExecuteFunction("ScriptGameInstance", "GetStatPoolsSystem",
            &statPoolsSystem, gameInstance)
            || !statPoolsSystem)
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[GetStatValue(pool)] GetStatPoolsSystem returned null");
            return -1.0f;
        }

        // 4) find the RTTI class and its getter
        auto* rtti = CRTTISystem::Get();
        if (!rtti)
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[GetStatValue(pool)] CRTTISystem not available");
            return -1.0f;
        }
        auto* poolsCls = rtti->GetClass("gameStatPoolsSystem");
        if (!poolsCls)
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[GetStatValue(pool)] RTTI class 'gameStatPoolsSystem' not found");
            return -1.0f;
        }
        auto* getFunc = poolsCls->GetFunction("GetStatPoolValue");
        if (!getFunc)
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[GetStatValue(pool)] function 'GetStatPoolValue' not found");
            return -1.0f;
        }

        // 5) call into StatPoolsSystem
        float outValue = 0.0f;
        StackArgs_t args;
        args.emplace_back(nullptr, &playerID);
        args.emplace_back(nullptr, &poolType);
        args.emplace_back(nullptr, &playerHandle);

        if (!ExecuteFunction(statPoolsSystem, getFunc, &outValue, args))
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[GetStatValue(pool)] ExecuteFunction failed");
            return -1.0f;
        }

        return outValue;
    }

    bool SetStatValue(RED4ext::game::data::StatPoolType poolType, float newValue)
    {
        using namespace RED4ext;

        // 1) get the player
        Handle<IScriptable> playerHandle;
        ent::EntityID playerID;
        if (!GetPlayerHandleAndID(playerHandle, playerID))
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[SetStatValue] failed to get player handle/ID");
            return false;
        }

        // 2) get the game instance
        ScriptGameInstance gameInstance;
        if (!ExecuteGlobalFunction("GetGameInstance", &gameInstance))
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[SetStatValue] GetGameInstance failed");
            return false;
        }

        // 3) grab the StatPoolsSystem
        Handle<game::StatPoolsSystem> statPoolsSystem;
        if (!ExecuteFunction("ScriptGameInstance", "GetStatPoolsSystem",
            &statPoolsSystem, gameInstance)
            || !statPoolsSystem)
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[SetStatValue] GetStatPoolsSystem returned null");
            return false;
        }

        // 4) find the RTTI class for gameStatPoolsSystem
        auto* rtti = CRTTISystem::Get();
        if (!rtti)
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[SetStatValue] CRTTISystem not available");
            return false;
        }
        auto* poolsCls = rtti->GetClass("gameStatPoolsSystem");
        if (!poolsCls)
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[SetStatValue] RTTI class 'gameStatPoolsSystem' not found");
            return false;
        }

        // 5) lookup the RequestSettingStatPoolValue function
        auto* reqFunc = poolsCls->GetFunction("RequestSettingStatPoolValue");
        if (!reqFunc)
        {
            loghandler::sdk->logger->Error(loghandler::handle,
                "[SetStatValue] function 'RequestSettingStatPoolValue' not found");
            return false;
        }

        // 6) build args: (EntityID, StatPoolType, newValue, playerHandle, propagate)
        bool propagate = false;
        StackArgs_t args;
        args.emplace_back(nullptr, &playerID);
        args.emplace_back(nullptr, &poolType);
        args.emplace_back(nullptr, &newValue);
        args.emplace_back(nullptr, &playerHandle);
        args.emplace_back(nullptr, &propagate);

        // 7) execute
        ExecuteFunction(statPoolsSystem, reqFunc, nullptr, args);
        return true;
    }

    void InitializeUIStyle() {
        using namespace UI;
        auto& colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(Colors::Text);
        colors[ImGuiCol_TextDisabled] = ImGui::ColorConvertU32ToFloat4(Colors::MutedText);
        colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(Colors::Background);
        colors[ImGuiCol_ChildBg] = ImGui::ColorConvertU32ToFloat4(Colors::FrameBg);
        colors[ImGuiCol_PopupBg] = ImGui::ColorConvertU32ToFloat4(Colors::FrameBg);
        colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(Colors::Border);

        colors[ImGuiCol_FrameBg] = ImGui::ColorConvertU32ToFloat4(IM_COL32(40, 45, 55, 255));
        colors[ImGuiCol_FrameBgHovered] = ImGui::ColorConvertU32ToFloat4(IM_COL32(80, 110, 255, 180));
        colors[ImGuiCol_FrameBgActive] = ImGui::ColorConvertU32ToFloat4(IM_COL32(100, 140, 255, 220));

        colors[ImGuiCol_TitleBg] = ImGui::ColorConvertU32ToFloat4(Header::TitleBar);
        colors[ImGuiCol_TitleBgActive] = ImGui::ColorConvertU32ToFloat4(Colors::FrameBg);

        colors[ImGuiCol_SliderGrab] = ImGui::ColorConvertU32ToFloat4(IM_COL32(150, 200, 255, 255));
        colors[ImGuiCol_SliderGrabActive] = ImGui::ColorConvertU32ToFloat4(IM_COL32(180, 220, 255, 255));

        colors[ImGuiCol_Button] = ImGui::ColorConvertU32ToFloat4(IM_COL32(40, 45, 55, 255));
        colors[ImGuiCol_ButtonHovered] = ImGui::ColorConvertU32ToFloat4(IM_COL32(80, 110, 255, 180));
        colors[ImGuiCol_ButtonActive] = ImGui::ColorConvertU32ToFloat4(IM_COL32(100, 140, 255, 220));

        colors[ImGuiCol_CheckMark] = ImGui::ColorConvertU32ToFloat4(Colors::Highlight);
        colors[ImGuiCol_Header] = ImGui::ColorConvertU32ToFloat4(Colors::FrameBg);
        colors[ImGuiCol_HeaderHovered] = ImGui::ColorConvertU32ToFloat4(Colors::Highlight);
        colors[ImGuiCol_HeaderActive] = ImGui::ColorConvertU32ToFloat4(Colors::Highlight);

        colors[ImGuiCol_Separator] = ImGui::ColorConvertU32ToFloat4(Colors::Border);
        colors[ImGuiCol_ResizeGrip] = ImGui::ColorConvertU32ToFloat4(Colors::Highlight);
        colors[ImGuiCol_ResizeGripHovered] = ImGui::ColorConvertU32ToFloat4(Colors::Highlight);
        colors[ImGuiCol_ResizeGripActive] = ImGui::ColorConvertU32ToFloat4(Colors::Highlight);

        colors[ImGuiCol_Tab] = ImGui::ColorConvertU32ToFloat4(Colors::FrameBg);
        colors[ImGuiCol_TabHovered] = ImGui::ColorConvertU32ToFloat4(Colors::Highlight);
        colors[ImGuiCol_TabActive] = ImGui::ColorConvertU32ToFloat4(Colors::Highlight);
        colors[ImGuiCol_TabUnfocused] = ImGui::ColorConvertU32ToFloat4(Colors::FrameBg);
        colors[ImGuiCol_TabUnfocusedActive] = ImGui::ColorConvertU32ToFloat4(Colors::FrameBg);
    }


    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration;

    static float currentHealth = -1.f;
    static float currentStamina = -1.f;
    static float currentArmor = -1.f;
    static bool  poolValuesInit = false;

    // toggles + target values
    static bool  healthEnabled = false;
    static float healthValue = 100.0f;

    static bool  staminaEnabled = false;
    static float staminaValue = 100.0f;

    static bool  armorEnabled = false;
    static float armorValue = 0.0f;

    void NativeTick()
    {
        using namespace RED4ext;

        // grab the *current* pool values once on first tick
        if (!poolValuesInit)
        {
            currentHealth = GetStatValue(game::data::StatPoolType::Health);
            currentStamina = GetStatValue(game::data::StatPoolType::Stamina);
            currentArmor = GetStatValue(game::data::StatPoolType::Armor);
            poolValuesInit = true;
        }

        // set pools whenever their toggle is on
        if (healthEnabled)
            SetStatValue(game::data::StatPoolType::Health, healthValue);

        if (staminaEnabled)
            SetStatValue(game::data::StatPoolType::Stamina, staminaValue);

        if (armorEnabled)
            SetStatValue(game::data::StatPoolType::Armor, armorValue);
    }


    namespace DebugUI {
        extern void DrawStyleDebugMenu();
    }

    void DrawMainMenu()
    {
        using namespace controls;
        using namespace render::ui;

        HandleInputTick();
        if (!controls::menuOpen)
            return;

        // full-screen background window
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowBgAlpha(0.f);
        ImGui::Begin("##BackgroundWindow", nullptr, flags);
        background::DrawBackgroundWindow();

        // pool sliders + toggles
        DrawFloatOption("Health", healthValue, 0.0f, 10000.0f, 1.0f, true, healthEnabled);
        DrawFloatOption("Stamina", staminaValue, 0.0f, 10000.0f, 1.0f, true, staminaEnabled);
        DrawFloatOption("Armor", armorValue, 0.0f, 10000.0f, 1.0f, true, armorEnabled);

        DrawToggleOption("Enable Health Pool", healthEnabled);
        DrawToggleOption("Enable Stamina Pool", staminaEnabled);
        DrawToggleOption("Enable Armor Pool", armorEnabled);

        // current values display + reset
        DrawOptionTextRAW("Current Health:  " + std::format("{:.1f}", currentHealth));
        DrawOptionTextRAW("Current Stamina: " + std::format("{:.1f}", currentStamina));
        DrawOptionTextRAW("Current Armor:   " + std::format("{:.1f}", currentArmor));
        if (DrawOptionTextRAW("Reset Pools"))
            poolValuesInit = false;

        ImGui::End();
        DebugUI::DrawStyleDebugMenu();
    }





    namespace DebugUI {

        void DrawColorEditor(const char* label, ImU32& color, const ImU32 defaultColor) {
            float col[4] = {
                ((color >> IM_COL32_R_SHIFT) & 0xFF) / 255.0f,
                ((color >> IM_COL32_G_SHIFT) & 0xFF) / 255.0f,
                ((color >> IM_COL32_B_SHIFT) & 0xFF) / 255.0f,
                ((color >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f
            };

            if (ImGui::ColorEdit4(label, col)) {
                color = IM_COL32((int)(col[0] * 255), (int)(col[1] * 255), (int)(col[2] * 255), (int)(col[3] * 255));
            }

            ImGui::SameLine();
            if (ImGui::Button((std::string("Reset##") + label).c_str())) {
                color = defaultColor;
            }
        }

        void DrawStyleDebugMenu() {
            if (!ImGui::Begin("UI Style Debug")) return;

            ImGui::Text("-- Colors");
            DrawColorEditor("Text", UI::Colors::Text, IM_COL32(220, 230, 255, 255));
            DrawColorEditor("MutedText", UI::Colors::MutedText, IM_COL32(130, 140, 160, 255));
            DrawColorEditor("Background", UI::Colors::Background, IM_COL32(10, 12, 18, 255));
            DrawColorEditor("FrameBg", UI::Colors::FrameBg, IM_COL32(20, 22, 30, 255));
            DrawColorEditor("Border", UI::Colors::Border, IM_COL32(70, 75, 90, 255));
            DrawColorEditor("Highlight", UI::Colors::Highlight, IM_COL32(100, 200, 255, 255));
            DrawColorEditor("ActiveHighlight", UI::Colors::ActiveHighlight, IM_COL32(100, 200, 255, 90));
            DrawColorEditor("HoverBg", UI::Colors::HoverBg, IM_COL32(45, 60, 85, 160));
            DrawColorEditor("Active", UI::Colors::Active, IM_COL32(55, 120, 180, 255));
            DrawColorEditor("Grab", UI::Colors::Grab, IM_COL32(160, 220, 255, 255));

            ImGui::Separator();
            ImGui::Text("-- Layout");
            ImGui::SliderFloat("Padding", &UI::Layout::Padding, 0.0f, 30.0f);
            ImGui::SliderFloat("FrameRounding", &UI::Layout::FrameRounding, 0.0f, 20.0f);
            ImGui::SliderFloat("FrameHeight", &UI::Layout::FrameHeight, 10.0f, 50.0f);
            ImGui::SliderFloat("OptionHeight", &UI::Layout::OptionHeight, 10.0f, 50.0f);
            ImGui::SliderFloat("OptionPaddingX", &UI::Layout::OptionPaddingX, 0.0f, 30.0f);
            ImGui::SliderFloat("OptionPaddingY", &UI::Layout::OptionPaddingY, 0.0f, 30.0f);
            ImGui::SliderFloat("CheckboxSize", &UI::Layout::CheckboxSize, 5.0f, 30.0f);
            ImGui::SliderFloat("SliderWidth", &UI::Layout::SliderWidth, 20.0f, 300.0f);
            ImGui::SliderFloat("LabelOffsetX", &UI::Layout::LabelOffsetX, 0.0f, 30.0f);

            
            ImGui::Separator();
            ImGui::Text("-- Toggle");
            DrawColorEditor("Toggle::OnColor", UI::Toggle::OnColor, UI::Colors::Highlight);
            DrawColorEditor("Toggle::OffColor", UI::Toggle::OffColor, IM_COL32(40, 40, 60, 255));
            ImGui::SliderFloat("Toggle::Size", &UI::Toggle::Size, 5.0f, 30.0f);
            ImGui::SliderFloat("Toggle::Rounding", &UI::Toggle::Rounding, 0.0f, 10.0f);
            ImGui::SliderFloat("Toggle::ToggleOffsetX", &UI::Toggle::ToggleOffsetX, 0.0f, 10.0f);

            ImGui::Separator();
            ImGui::Text("-- Slider");
            DrawColorEditor("Slider::BgColor", UI::Slider::BgColor, UI::Colors::FrameBg);
            DrawColorEditor("Slider::GrabColor", UI::Slider::GrabColor, UI::Colors::Grab);
            ImGui::SliderFloat("Slider::Height", &UI::Slider::Height, 5.0f, 20.0f);
            ImGui::SliderFloat("Slider::Rounding", &UI::Slider::Rounding, 0.0f, 10.0f);

            ImGui::Separator();
            ImGui::Text("-- Input");
            DrawColorEditor("Input::Bg", UI::Input::Bg, IM_COL32(20, 20, 28, 255));
            DrawColorEditor("Input::Placeholder", UI::Input::Placeholder, IM_COL32(160, 160, 160, 120));
            ImGui::SliderFloat("Input::Height", &UI::Input::Height, 10.0f, 40.0f);

            ImGui::Separator();
            ImGui::Text("-- Header");
            DrawColorEditor("Header::TitleBar", UI::Header::TitleBar, IM_COL32(20, 22, 28, 255));
            DrawColorEditor("Header::Footer", UI::Header::Footer, IM_COL32(25, 25, 38, 255));
            ImGui::SliderFloat("Header::Height", &UI::Header::Height, 20.0f, 60.0f);

            ImGui::Separator();
            ImGui::Text("-- Scroll");
            ImGui::SliderInt("Scroll::MaxVisibleOptions", &UI::Scroll::MaxVisibleOptions, 1, 20);
            ImGui::SliderFloat("Scroll::ScrollSpeed", &UI::Scroll::ScrollSpeed, 1.0f, 10.0f);

            ImGui::Separator();
            ImGui::Text("-- Animation");
            ImGui::SliderFloat("Animation::SmoothSpeed", &UI::Animation::SmoothSpeed, 0.01f, 1.0f);

            ImGui::End();
        }
    }


}