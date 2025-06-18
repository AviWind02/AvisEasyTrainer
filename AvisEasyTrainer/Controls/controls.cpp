#include "pch.h"
#include "controls.h"



namespace controls
{

    // Cooldown
    static uint64_t lastKeyTick = 0;
    static const int scrollDelay = 150;

    bool IsKeyPressed(int key)
    {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }

    bool Up()
    {
        return IsKeyPressed(VK_UP) || IsKeyPressed(VK_NUMPAD8) || ImGui::IsKeyPressed(ImGuiKey_GamepadDpadUp);
    }

    bool Back()
    {
        return IsKeyPressed(VK_BACK) || IsKeyPressed(VK_NUMPAD0) || ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight);
    }

    bool Down()
    {
        return IsKeyPressed(VK_DOWN) || IsKeyPressed(VK_NUMPAD2) || ImGui::IsKeyPressed(ImGuiKey_GamepadDpadDown);
    }

    bool Left()
    {
        return IsKeyPressed(VK_LEFT) || IsKeyPressed(VK_NUMPAD4) || ImGui::IsKeyPressed(ImGuiKey_GamepadDpadLeft);
    }

    bool Right()
    {
        return IsKeyPressed(VK_RIGHT) || IsKeyPressed(VK_NUMPAD6) || ImGui::IsKeyPressed(ImGuiKey_GamepadDpadRight);
    }

    bool Select()
    {
        return IsKeyPressed(VK_RETURN) || IsKeyPressed(VK_NUMPAD5) || ImGui::IsKeyPressed(ImGuiKey_GamepadFaceDown);
    }

    bool Misc()
    {
        return IsKeyPressed(VK_CONTROL) || ImGui::IsKeyPressed(ImGuiKey_GamepadL3);
    }

    bool ToggleMenu()
    {
        return IsKeyPressed(VK_F4) || (ImGui::IsKeyPressed(ImGuiKey_GamepadFaceDown) && ImGui::IsKeyPressed(ImGuiKey_GamepadR1));
    }
    bool ToggleMouse()
    {
        return IsKeyPressed('X');
    }

 



    void HandleInputTick()
    {
        uint64_t now = GetTickCount64();

        leftPressed = false;
        rightPressed = false;
        selectPressed = false;
        miscPressed = false;

        if (ToggleMenu() && now - lastKeyTick > scrollDelay)
        {
            menuOpen = !menuOpen;
            lastKeyTick = now;
        }

        if (!menuOpen) 
            return;

        if (Up() && now - lastKeyTick > scrollDelay)
        {
            currentOption = (currentOption > 1) ? currentOption - 1 : optionIndex;
            lastKeyTick = now;
        }

        if (Down() && now - lastKeyTick > scrollDelay)
        {
            currentOption = (currentOption < optionIndex) ? currentOption + 1 : 1;
            lastKeyTick = now;
        }

        if (Left() && now - lastKeyTick > scrollDelay)
        {
            leftPressed = true;
            lastKeyTick = now;
        }

        if (Right() && now - lastKeyTick > scrollDelay)
        {
            rightPressed = true;
            lastKeyTick = now;
        }

        if (Select() && now - lastKeyTick > scrollDelay)
        {
            selectPressed = true;
            lastKeyTick = now;
        }

        if (Back() && now - lastKeyTick > scrollDelay)
        {
			render::ui::CloseSubMenu();
            lastKeyTick = now;
        }

        if (ToggleMouse() && now - lastKeyTick > scrollDelay)
        {
            mouseToggle = !mouseToggle;
            lastKeyTick = now;
        }

        if (Misc() && now - lastKeyTick > scrollDelay)
        {
            miscPressed = true;
            lastKeyTick = now;
        }

        optionIndex = 0;
    }

}