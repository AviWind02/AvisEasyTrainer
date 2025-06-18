#include "pch.h"
#include <set>

#include "Features/VehicleFeat/vehiclefeature.h"
#include "Base/Natives/vehicleclass.h"

#include "vehicleview.h"

using namespace render::ui;

namespace view::vehicle {
    namespace vehicleunlocks {


        enum class FilterMode { None, All, Category, Brand, Affiliation };

        FilterMode currentMode = FilterMode::None;
        std::string selectedValue;

        void FilteredVehicleView()
        {
            using namespace feature::vehicleoptions;

            for (const auto& v : allVehicles)
            {
                const std::string& modelName = v.modelName.empty() ? v.recordID : v.modelName;

                bool match = false;
                switch (currentMode)
                {
                case FilterMode::All: match = true; break;
                case FilterMode::Category: match = (v.category == selectedValue); break;
                case FilterMode::Brand: match = (v.brandName == selectedValue); break;
                case FilterMode::Affiliation: match = (v.affiliation == selectedValue); break;
                default: break;
                }

                if (!match)
                    continue;

                bool& toggle = vehicleToggleStates[v.recordID];
                std::string tooltip = "Unlock " + modelName + " and add it to your player vehicle list. Disable to remove it. Make sure you're not inside the vehicle before disabling.";
                if (buttons::Toggle(modelName, toggle, tooltip))
                {
                    RequestVehicleToggle(v.recordID);
                }
            }
        }    SubMenu vehicleUnlockFilteredMenu{ "Filtered Vehicles", &FilteredVehicleView };


        void VehicleUnlockMainView()
        {
            using namespace feature::vehicleoptions;
            std::set<std::string> shownCategories, shownBrands, shownAffiliations;


            if (buttons::Submenu("All Vehicles", vehicleUnlockFilteredMenu))
            {
                currentMode = FilterMode::All;
                selectedValue.clear();
            }

            if (buttons::Submenu("Player Vehicles", vehicleUnlockFilteredMenu))
            {
                currentMode = FilterMode::Affiliation;
                selectedValue = "Player";
            }

            buttons::Break("", "Filter by Category");

            for (const auto& v : allVehicles)
            {
                if (shownCategories.insert(v.category).second)
                {
                    if (buttons::Submenu(v.category.c_str(), vehicleUnlockFilteredMenu))
                    {
                        currentMode = FilterMode::Category;
                        selectedValue = v.category;
                    }
                }
            }

            buttons::Break("", "Filter by Brand Name");

            for (const auto& v : allVehicles)
            {
                if (v.brandName.empty())
                    continue;

                if (shownBrands.insert(v.brandName).second)
                {
                    if (buttons::Submenu(v.brandName.c_str(), vehicleUnlockFilteredMenu))
                    {
                        currentMode = FilterMode::Brand;
                        selectedValue = v.brandName;
                    }
                }
            }

            buttons::Break("", "Filter by Affiliation");

            for (const auto& v : allVehicles)
            {
                if (v.affiliation.empty() || v.affiliation == "Player" || v.affiliation == "Unknown")
                    continue;

                if (shownAffiliations.insert(v.affiliation).second)
                {
                    if (buttons::Submenu(v.affiliation.c_str(), vehicleUnlockFilteredMenu))
                    {
                        currentMode = FilterMode::Affiliation;
                        selectedValue = v.affiliation;
                    }
                }
            }
        }

       



    }
}