#pragma once
namespace Controls {
    // State
    inline bool menuOpen = true;
    inline bool mouseToggle = false;

    inline int optionIndex = 0;
    inline int currentOption = 1;

    // State for one-frame buttons
    inline bool leftPressed = false;
    inline bool rightPressed = false;
    inline bool selectPressed = false;
    inline bool miscPressed = false;

    // Input checks
    bool Up();
    bool Down();
    bool Left();
    bool Right();
    bool Select();
    bool Misc();
    bool ToggleMenu();

    // Tick handler
    void HandleInputTick();


    namespace InputHook
    {
        void Init(HWND hwnd);
        void Remove(HWND hwnd);

        void SetMouseTrap(bool enable);
        bool IsMouseTrapped();

        namespace Draw
        {
            inline int g_MouseDeltaX = 0;
            inline int g_MouseDeltaY = 0;

            inline ImVec2 g_CustomCursorPos = { 500.f, 500.f };
            inline POINT g_LastCursorPos = {};
            inline bool g_CursorInitialized = false;
            inline float g_CursorSensitivity = 1.0f;
        }
    }

}