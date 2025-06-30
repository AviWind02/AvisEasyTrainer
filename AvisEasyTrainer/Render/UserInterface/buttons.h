#pragma once

namespace UI {
	namespace Buttons {

        inline bool Option(const std::string& label, const std::string& tip = "", std::function<void()> action = [] {})
        {
            if (DrawOptionTextRAW(label, "", "", tip)) {
                action();
                return true;
            }
            return false;
        }

        inline bool OptionExtended(const std::string& label, const std::string& centerText, const std::string& rightText, const std::string& tip = "", std::function<void()> action = [] {})
        {
            if (DrawOptionTextRAW(label, centerText, rightText, tip)) {
                action();
                return true;
            }
            return false;
        }

        inline bool Break(const std::string& label, const std::string& centerLabel = "", const std::string& rightLabel = "")
        {
            return DrawOptionTextRAW("#BREAK" + label, centerLabel, rightLabel);
        }

        inline bool Submenu(const std::string& label, const SubMenu& submenu, const std::string& tip = "", std::function<void()> action = [] {})
        {
            if (DrawOptionTextRAW(label, "", ICON_FA_ARROW_RIGHT, tip)) {
                OpenSubMenu(submenu);
                action();
                return true;
            }
            return false;
        }

        inline bool Toggle(const std::string& label, bool& toggle, const std::string& tip = "", std::function<void()> action = [] {})
        {
            if (DrawToggleOption(label, toggle, tip)) {
                action();
                return true;
            }
            return false;
        }

        inline bool Float(const std::string& label, float& value, float min, float max, float step, const std::string& tip = "", std::function<void()> action = [] {})
        {
            static bool dummy = false;
            if (DrawFloatOption(label, value, min, max, step, false, dummy, tip)) {
                action();
                return true;
            }
            return false;
        }

        inline bool FloatToggle(const std::string& label, float& value, float min, float max, float step, bool& toggle, const std::string& tip = "", std::function<void()> action = [] {})
        {
            if (DrawFloatOption(label, value, min, max, step, true, toggle, tip)) {
                action();
                return true;
            }
            return false;
        }

        inline bool Int(const std::string& label, int& value, int min, int max, int step, const std::string& tip = "", std::function<void()> action = [] {})
        {
            static bool dummy = false;
            if (DrawIntOption(label, value, min, max, step, false, dummy, tip)) {
                action();
                return true;
            }
            return false;
        }

        inline bool IntToggle(const std::string& label, int& value, int min, int max, int step, bool& toggle, const std::string& tip = "", std::function<void()> action = [] {})
        {
            if (DrawIntOption(label, value, min, max, step, true, toggle, tip)) {
                action();
                return true;
            }
            return false;
        }

	}
}