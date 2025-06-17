#include "pch.h"
#include <set>

#include "Features/TeleportFeats/teleportfeature.h"

#include "teleportview.h"

using namespace render::ui;

namespace view::teleport {

    static std::string selectedCategory;


    void TeleportFilteredView()
    {
        using namespace feature::teleoptions;

        for (const auto& loc : teleportLocations)
        {
            if (loc.category != selectedCategory)
                continue;

            if (buttons::OptionExtended(loc.name, "", ICON_FA_MAP_MARKED))
            {
                RequestTeleport(loc.position);
            }
        }
    }
    SubMenu teleportFilteredMenu{ "Teleport Locations", &TeleportFilteredView };


    void TeleportMainView()
    {
        using namespace feature::teleoptions;

        static int forwardDistance = 2;
        static int upwardDistance = 0;

        buttons::Int("Forward Distance", forwardDistance, 1, 25, 1);
        //buttons::Int("Upward Distance", upwardDistance, 0, 20, 1);

        if (buttons::Option("Teleport Forward", "Move forward based on rotation"))
        {
            TeleportForward(static_cast<float>(forwardDistance));
            //TeleportUp(static_cast<float>(upwardDistance));
        }

        std::set<std::string> shownCategories;

        buttons::Break("", "Teleport Categories");

        for (const auto& loc : teleportLocations)
        {
            if (shownCategories.insert(loc.category).second)
            {
                if (buttons::Submenu(loc.category.c_str(), teleportFilteredMenu))
                {
                    selectedCategory = loc.category;
                }
            }
        }
    }

}