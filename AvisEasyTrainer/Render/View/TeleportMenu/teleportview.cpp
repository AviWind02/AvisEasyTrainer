#include "pch.h"
#include <set>

#include "Features/TeleportFeats/teleportfeature.h"

#include "teleportview.h"

using namespace UI;
using namespace Feature::TeleyOptions;

namespace View::Teleport {

    static std::string selectedCategory;


    void TeleportFilteredView()
    {

        for (const auto& loc : teleportLocations)
        {
            if (loc.category != selectedCategory)
                continue;

            if (Buttons::OptionExtended(loc.name, "", ICON_FA_MAP_MARKED, "Teleport created by: " + loc.creator))
            {
                RequestTeleport(loc.position);
            }
        }
    }
    SubMenu teleportFilteredMenu{ "Teleport Locations", &TeleportFilteredView };


    void TeleportMainView()
    {
        static int forwardDistance = 2;
        static int upwardDistance = 0;

        Buttons::Int("Forward Distance", forwardDistance, 1, 25, 1);
        //Buttons::Int("Upward Distance", upwardDistance, 0, 20, 1);

        if (Buttons::Option("Teleport Forward", "Move forward based on rotation"))
        {
            TeleportForward(static_cast<float>(forwardDistance));
            //TeleportUp(static_cast<float>(upwardDistance));
        }

        Buttons::Break("", "Teleport Categories");

        std::set<std::string> uniqueCategories;
        for (const auto& loc : teleportLocations)
            uniqueCategories.insert(loc.category);

        std::vector<std::string> sortedCategories(uniqueCategories.begin(), uniqueCategories.end());
        std::sort(sortedCategories.begin(), sortedCategories.end());

        for (const auto& category : sortedCategories)
        {
            if (Buttons::Submenu(category.c_str(), teleportFilteredMenu))
            {
                selectedCategory = category;
            }
        }
    }

}