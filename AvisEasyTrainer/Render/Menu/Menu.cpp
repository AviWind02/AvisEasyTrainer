#include "pch.h"

#include "Controls/controls.h"
#include "Render/Hooks/d3d12globals.h"
#include "Render/UserInterface/Style.h"



#include "Features/PlayerFeats/playerfeature.h"



namespace render::ui
{

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

    void NativeTick()
    {
        feature::playeroptions::Tick();
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
        {
            background::DrawBackgroundWindow();
            DrawToggleOption("Godmode", feature::playeroptions::tickGodmode, "Refills health to 100 constantly");

            DrawToggleOption("Health Regen", feature::playeroptions::tickGodHealthRegen, "Applies extreme passive health regeneration and boosts base health.");
            DrawToggleOption("Armor Boost", feature::playeroptions::tickGodArmor, "Increases armor to near-invincible levels.");
            DrawToggleOption("Fall Damage Reduction", feature::playeroptions::tickGodFallDamage, "Negates most fall damage.");
            DrawToggleOption("Damage Resistances", feature::playeroptions::tickGodResistances, "Maximizes all elemental and physical resistances.");
            DrawToggleOption("Combat Regen", feature::playeroptions::tickGodCombatRegen, "Enables health regeneration while in combat.");
            DrawToggleOption("Visibility Rate Reduction", feature::playeroptions::tickdetectionRate,
                "Lowers how easily NPCs detect you. You're still visible to them and can get detected up close or if in combat.");
            DrawToggleOption("tickTest", feature::playeroptions::tickTest, "");
			DrawFloatOption("Jump Height", feature::playeroptions::jumpHeight, 0.1f, 15.f, 0.5f, true, feature::playeroptions::tickSuperJump,  "Adjusts jump height.\n"
				"Set to 0.0 to disable super jump.");
            
            DrawToggleOption("Trace Rate Reduction", feature::playeroptions::tickTraceRatelow, "Reduces disables NPC trace rate mechanics.");

            DrawToggleOption("Unlimited Stamina", feature::playeroptions::tickUnlimitedStamina, "Keeps stamina maxed");
            DrawToggleOption("Unlimited Memory", feature::playeroptions::tickUnlimitedMemory,
                "Memory resets to 100% when exiting and re-entering scanner mode.\n"
                "Effectively gives infinite RAM outside of active Quickhacks.");
            
            DrawToggleOption("Unlimited Oxygen", feature::playeroptions::tickUnlimitedOxygen, "Keeps oxygen at 100%");

            DrawToggleOption("Heal Item Cooldown Reduction", feature::playeroptions::tickHealItemCooldown, "Removes cooldown between healing item uses.");
            DrawToggleOption("Grenade Cooldown Reduction", feature::playeroptions::tickGrenadeCooldown, "Removes cooldown between grenade throws.");
            DrawToggleOption("Projectile Launcher Cooldown", feature::playeroptions::tickProjectileCooldown, "Allows spamming projectile launcher.");
            DrawToggleOption("Cloak Cooldown Reduction", feature::playeroptions::tickCloakCooldown, "Rapidly regenerates cloak charges and duration.");
            DrawToggleOption("Sandevistan Cooldown Reduction", feature::playeroptions::tickSandevistanCooldown, "Reduces Sandevistan cooldown dramatically.");
            DrawToggleOption("Berserk Cooldown Reduction", feature::playeroptions::tickBerserkCooldown, "Allows frequent Berserk activations.");
            DrawToggleOption("Kerenzikov Cooldown Reduction", feature::playeroptions::tickKerenzikovCooldown, "Eliminates cooldown on Kerenzikov dodges.");
            DrawToggleOption("Overclock Cooldown Reduction", feature::playeroptions::tickOverclockCooldown, "Greatly boosts overclock regen and disables cooldown.");
            DrawToggleOption("Quickhack Cooldown Reduction", feature::playeroptions::tickQuickhackCooldown, "Minimizes cooldown between Quickhacks.");
            DrawToggleOption("Quickhack Cost Reduction", feature::playeroptions::tickQuickhackCost, "Reduces RAM cost of Quickhacks to almost zero.");
            DrawToggleOption("Memory Regeneration Boost", feature::playeroptions::tickMemoryRegeneration, "Massively increases RAM regen rate.");
			DrawIntOption("Memory Value", feature::playeroptions::memoryValue, 1, 256, 1, true, feature::playeroptions::tickMemoryEdit,
				"Sets the maximum memory value for Quickhacks.\n"
				"Set to 16 for default value.\n"
				"Set to 64 for maximum value.");
        }
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