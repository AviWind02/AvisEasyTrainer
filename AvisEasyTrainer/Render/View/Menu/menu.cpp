#include "pch.h"

#include "Render/Hooks/d3d12globals.h"
#include "Render/UserInterface/Style.h"

#include "Features/PlayerFeats/playerfeature.h"
#include "Features/TeleportFeats/teleportfeature.h"
#include "Features/VehicleFeat/vehiclefeature.h"
#include "Features/WorldFeat/WorldFeature.h"

#include "Base/Natives/Vehicle.h"


#include "Render/View/VehicleMenu/vehicleView.h"
#include "Render/View/TeleportMenu/teleportview.h"
#include "Render/View/WorldMenu/WorldView.h"

namespace UI
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


    void UpdateStatesOnVehicleMenuView()
    {
        static bool hasCheckedUnlockStates = false;

        if (!menuStack.empty() &&
            menuStack.back().title == view::vehicle::vehicleunlocks::vehicleUnlockMenu.title &&
            !hasCheckedUnlockStates)
        {
            Feature::VehicleOptions::UpdateVehicleUnlockStates();
            hasCheckedUnlockStates = true;
        }

    }

    bool tickTest = false;
    void NativeTick()
    {
        Feature::PlayerOptions::Tick();
        Feature::TeleyOptions::Tick();
        Feature::VehicleOptions::Tick();
        Feature::World::Time::Tick();
        Feature::World::Weather::Tick();
        UpdateStatesOnVehicleMenuView();


		if (tickTest) {
			tickTest = false;
		}
    }


    namespace DebugUI {
        extern void DrawStyleDebugMenu();
    }

    void SelfView()
    {

        buttons::Toggle("Unlimited health", Feature::PlayerOptions::tickGodmode, "Refills health to 100 constantly");
        buttons::Toggle("Unlimited Stamina",Feature::PlayerOptions::tickUnlimitedStamina, "Keeps stamina maxed.");
        buttons::Toggle("Unlimited Oxygen",Feature::PlayerOptions::tickUnlimitedOxygen, "Keeps oxygen at 100%");
        buttons::Toggle("NoClip Mode",Feature::PlayerOptions::tickNoClip, "Move freely in any direction without collisions.");
        buttons::Toggle("Quicksilver's Sandevistan",Feature::PlayerOptions::tickQuicksilver, "Set time scale to near-zero, and extends Sandevistan duration.", [] {
           Feature::PlayerOptions::tickSandevistanDuration = false;
           Feature::PlayerOptions::tickSandevistanTimeScale = false; });
        buttons::Toggle("Never Wanted",Feature::PlayerOptions::PlayerWanted::tickNeverWanted, "Prevents police from pursuing you.",
            [] {Feature::PlayerOptions::PlayerWanted::SetNeverWantedLevel(Feature::PlayerOptions::PlayerWanted::tickNeverWanted); });
        buttons::IntToggle("Wanted Level",Feature::PlayerOptions::PlayerWanted::heldWantedLevel, 1, 5, 1,Feature::PlayerOptions::PlayerWanted::tickHoldWanted,
            "Sets your wanted level. If toggled on, it will be held.",
            [] {Feature::PlayerOptions::PlayerWanted::SetHoldWantedLevel(Feature::PlayerOptions::PlayerWanted::tickHoldWanted); });
        buttons::Option("Clear Wanted level", "Temporarily disables the police system to reset wanted level.", [] {Feature::PlayerOptions::PlayerWanted::tickClearWanted = true; });
        buttons::FloatToggle("Player Max Speed",Feature::PlayerOptions::maxSpeedValue, 1.0f, 15.f, 0.5f,Feature::PlayerOptions::tickPlayerMaxSpeed, "Set maximum player speed.");
        buttons::Toggle("Health Regen",Feature::PlayerOptions::tickGodHealthRegen, "Extreme passive health regeneration.");
        buttons::Toggle("Armor Boost",Feature::PlayerOptions::tickGodArmor, "Near invincible armor.");
        buttons::Toggle("Damage Resistances",Feature::PlayerOptions::tickGodResistances, "Maximizes all resistances.");
		buttons::IntToggle("Carry Capacity",Feature::PlayerOptions::carryCapacityValue, 1, 1000, 1,Feature::PlayerOptions::tickCarryCapacity, "Set carry capacity.");
        buttons::FloatToggle("Jump Height",Feature::PlayerOptions::jumpHeight, 0.1f, 150.f, 0.5f,Feature::PlayerOptions::tickSuperJump, "Set jump height.");
        buttons::Toggle("Combat Regen",Feature::PlayerOptions::tickGodCombatRegen, "Regen during combat.");
        buttons::Toggle("Unlimited Memory",Feature::PlayerOptions::tickUnlimitedMemory, "Full RAM when exiting scanner.");
        buttons::IntToggle("Memory Value",Feature::PlayerOptions::memoryValue, 1, 256, 1,Feature::PlayerOptions::tickMemoryEdit, "Set RAM max.");
        buttons::Toggle("Memory Regeneration Boost",Feature::PlayerOptions::tickMemoryRegeneration, "Massive RAM regen.");
       





    } SubMenu selfMenu{ "Self Menu", &SelfView };
    void ReductionView()
    {

        buttons::Toggle("Visibility Rate Reduction",Feature::PlayerOptions::tickdetectionRate, "Lowers how easily NPCs detect you. You're still visible to them and can get detected up close or if in combat.");
        buttons::Toggle("Fall Damage Reduction",Feature::PlayerOptions::tickGodFallDamage, "Negates most fall damage.");
        buttons::Toggle("Trace Rate Reduction",Feature::PlayerOptions::tickTraceRatelow, "Reduce trace mechanics.");
        buttons::Toggle("Heal Item Cooldown Reduction",Feature::PlayerOptions::tickHealItemCooldown, "No cooldown on healing.");
        buttons::Toggle("Grenade Cooldown Reduction",Feature::PlayerOptions::tickGrenadeCooldown, "No cooldown on grenades.");
        buttons::Toggle("Projectile Launcher Cooldown",Feature::PlayerOptions::tickProjectileCooldown, "Spam launcher freely.");
        buttons::Toggle("Cloak Cooldown Reduction",Feature::PlayerOptions::tickCloakCooldown, "Fast cloak recharge.");
        buttons::Toggle("Sandevistan Cooldown Reduction",Feature::PlayerOptions::tickSandevistanCooldown, "Short cooldown.");
        buttons::FloatToggle("Sandevistan Duration",Feature::PlayerOptions::sandevistanDurationValue, 1.f, 15.f, 0.25f,Feature::PlayerOptions::tickSandevistanDuration, "Multiplier for Sandevistan effect duration.");
        buttons::FloatToggle("Sandevistan Time Scale",Feature::PlayerOptions::sandevistanTimeScaleValue, 0.001f, 2.f, 0.005f,Feature::PlayerOptions::tickSandevistanTimeScale, "Multiplier for how time flows during Sandevistan.");
        buttons::Toggle("Berserk Cooldown Reduction",Feature::PlayerOptions::tickBerserkCooldown, "Frequent activations.");
        buttons::Toggle("Kerenzikov Cooldown Reduction",Feature::PlayerOptions::tickKerenzikovCooldown, "No cooldown.");
        buttons::Toggle("Overclock Cooldown Reduction",Feature::PlayerOptions::tickOverclockCooldown, "No overclock delay.");
        buttons::Toggle("Quickhack Cooldown Reduction",Feature::PlayerOptions::tickQuickhackCooldown, "Rapid quickhacks.");
        buttons::Toggle("Quickhack Cost Reduction",Feature::PlayerOptions::tickQuickhackCost, "Reduces RAM cost.");


    }SubMenu cooldownMenu{ "Cooldown & Reduction Menu", &ReductionView };


    void MainMenuView()
    {
        buttons::Submenu("Self Menu", selfMenu,
            "Enhance player stats like health, stamina, and resistances. Includes god mode, super jump, no-clip, memory boosts, and wanted level control.");

        buttons::Submenu("Vehicle Menu", view::vehicle::vehicleunlocks::vehicleUnlockMenu, "Unlock all vehicles and remove saved vehicles from your owned list.");
        buttons::Submenu("Cooldown & Reduction Menu", cooldownMenu, "Reduce or remove cooldowns for healing, grenades, Sandevistan, Berserk, cloak, and quickhacks.");
        buttons::Submenu("Teleport Menu", view::teleport::teleportMenu, "Teleport instantly preset locations  in Night City.");
        buttons::Submenu("Time Menu", view::world::timeview::timeMenu, "Adjust in-game time manually or synchronize with your real-world system clock.");
        buttons::Submenu("Weather Menu", view::world::weatherview::weatherControlMenu, "Change current weather.");



		buttons::Toggle("Test Feature", tickTest, "Test feature toggle for debugging purposes");
        buttons::Toggle("Test Player Feature",Feature::PlayerOptions::tickTest  , "");

    } SubMenu mainMenu{ "Main Menu",  &MainMenuView };
    

    void DrawMainMenu()
    {
        static bool initialized = false;
        if (!initialized) {
            OpenSubMenu(mainMenu);
            NotificationSystem::NotifyInfo("EasyTrainer initialized");
            initialized = true;
        }
        Controls::HandleInputTick();

        if (!Controls::menuOpen)
            return;

        // full-screen background window
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowBgAlpha(0.f);
        ImGui::Begin("##BackgroundWindow", nullptr, flags);
        {
            background::DrawTitleBar();
            background::DrawBackgroundWindow();
            if (!menuStack.empty()) {
                menuStack.back().view();
            }
            background::DrawFooter();
            NotificationSystem::Render();
            InfoBox::Render();
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