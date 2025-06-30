#include "pch.h"
#include <set>

#include "Features/VehicleFeat/vehiclefeature.h"
#include "Base/Natives/vehicle.h"

#include "vehicleview.h"

using namespace UI;
using namespace Feature::VehicleOptions;

namespace View::Vehicle {
    namespace Vehicleunlocks {


        enum class FilterMode { None, All, Category, Brand, Affiliation };

        FilterMode currentMode = FilterMode::None;
        std::string selectedValue;

        void FilteredVehicleView()
        {

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
                if (Buttons::Toggle(modelName, toggle, tooltip))
                {
                    RequestVehicleToggle(v.recordID);
                }
            }
        }    SubMenu vehicleUnlockFilteredMenu{ "Filtered Vehicles", &FilteredVehicleView };


        void VehicleUnlockMainView()
        {
            std::set<std::string> shownCategories, shownBrands, shownAffiliations;


            if (Buttons::Submenu("All Vehicles", vehicleUnlockFilteredMenu))
            {
                currentMode = FilterMode::All;
                selectedValue.clear();
            }

            if (Buttons::Submenu("Player Vehicles", vehicleUnlockFilteredMenu))
            {
                currentMode = FilterMode::Affiliation;
                selectedValue = "Player";
            }

            Buttons::Break("", "Filter by Category");

            for (const auto& v : allVehicles)
            {
                if (shownCategories.insert(v.category).second)
                {
                    if (Buttons::Submenu(v.category.c_str(), vehicleUnlockFilteredMenu))
                    {
                        currentMode = FilterMode::Category;
                        selectedValue = v.category;
                    }
                }
            }

            Buttons::Break("", "Filter by Brand Name");

            for (const auto& v : allVehicles)
            {
                if (v.brandName.empty())
                    continue;

                if (shownBrands.insert(v.brandName).second)
                {
                    if (Buttons::Submenu(v.brandName.c_str(), vehicleUnlockFilteredMenu))
                    {
                        currentMode = FilterMode::Brand;
                        selectedValue = v.brandName;
                    }
                }
            }

            Buttons::Break("", "Filter by Affiliation");

            for (const auto& v : allVehicles)
            {
                if (v.affiliation.empty() || v.affiliation == "Player" || v.affiliation == "Unknown")
                    continue;

                if (shownAffiliations.insert(v.affiliation).second)
                {
                    if (Buttons::Submenu(v.affiliation.c_str(), vehicleUnlockFilteredMenu))
                    {
                        currentMode = FilterMode::Affiliation;
                        selectedValue = v.affiliation;
                    }
                }
            }
        }

       



    }
}