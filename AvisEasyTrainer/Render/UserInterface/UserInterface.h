#pragma once

namespace render::ui
{

    struct MenuBox {
        ImVec2 pos = ImVec2(300.f, 200.f);
        ImVec2 size = ImVec2(420.f, 500.f);
        bool isDragging = false;
        bool isResizing = false;
        ImVec2 dragOffset;
        ImVec2 resizeOffset;

    }; extern MenuBox menuBox;
    
    void InitializeUIStyle();
    void DrawMainMenu();
    void NativeTick();


    namespace background {
        void DrawBackgroundWindow();
    }


    // Draws a centered label row with optional left/right text
        // Returns true if clicked (used for selection logic)
    bool DrawOptionTextRAW(const std::string& left, const std::string& center = "", const std::string& right = "");

    // Draws a boolean toggle option using a checkbox on the right
    // Parameters:
    // - label: The visible name of the option
    // - toggle: Reference to the boolean to toggle
    // - tip: (unused currently) Optional tooltip or description string
    bool DrawToggleOption(const std::string& label, bool& toggle, const std::string& tip = "");

    // Draws a float slider option
    // Parameters:
    // - label: Name of the option to display
    // - value: Float reference to change
    // - min / max: Slider bounds
    // - step: Step amount for keyboard input
    // - useToggle: Show a checkbox alongside
    // - toggle: Boolean reference if useToggle is true
    // - tip: (unused currently) Tooltip string
    bool DrawFloatOption(const std::string& label, float& value, float min, float max, float step, bool useToggle, bool& toggle, const std::string& tip = "");

    // Draws an integer slider option
    // Parameters:
    // - label: Name of the option to display
    // - value: Integer reference to change
    // - min / max: Slider bounds
    // - step: Step amount for keyboard input
    // - useToggle: Show a checkbox alongside
    // - toggle: Boolean reference if useToggle is true
    // - tip: (unused currently) Tooltip string
    bool DrawIntOption(const std::string& label, int& value, int min, int max, int step, bool useToggle, bool& toggle, const std::string& tip = "");
  
}
