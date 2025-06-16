#pragma once
#include <string>
#include <vector>
#include <functional>

namespace render::ui {


    struct SubMenu {
        std::string title;
        std::function<void()> view;
    };

    inline std::vector<SubMenu> menuStack;
    inline std::vector<int> optionStack;
    inline int currentMenuIndex = -1;

    inline void OpenSubMenu(const SubMenu& submenu) {
        menuStack.push_back(submenu);
        optionStack.push_back(controls::currentOption);

        if (optionStack.size() > 10)
            optionStack.erase(optionStack.begin());

        currentMenuIndex++;
        controls::currentOption = 1;
    }

    inline void CloseSubMenu() {
        if (menuStack.size() > 1) {
            menuStack.pop_back();
            currentMenuIndex--;

            if (!optionStack.empty()) {
                controls::currentOption = optionStack.back();
                optionStack.pop_back();
            }
            else {
                controls::currentOption = 1;
            }
        }
    }

    inline std::string GetBreadcrumbTitle() {
        std::string separator = " > ";
        std::string fullBreadcrumb;

        for (const auto& menu : menuStack) {
            fullBreadcrumb += menu.title + separator;
        }

        if (!fullBreadcrumb.empty()) {
            fullBreadcrumb.erase(fullBreadcrumb.size() - separator.size());
        }

        int maxChars = static_cast<int>(menuBox.size.x / 12.f);
        if (fullBreadcrumb.length() > maxChars) {
            size_t start = fullBreadcrumb.length() - maxChars;
            fullBreadcrumb = "..." + fullBreadcrumb.substr(start + 3);
        }

        return fullBreadcrumb;
    }

}
