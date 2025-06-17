#include "pch.h"
#include <set>

#include "Features/VehicleFeat/vehiclefeature.h"
#include "Base/Natives/vehicleclass.h"

#include "vehicleview.h"

using namespace render::ui;

namespace view::vehicle {
    namespace vehicleunlocks {


        enum class FilterMode { None, Category, Model, Affiliation };

        FilterMode currentMode = FilterMode::None;
        std::string selectedValue;


        void FilteredVehicleView()
        {
            using namespace feature::vehicleoptions;

            for (const auto& v : allVehicles)
            {
                std::string model = v.modelName.empty() ? v.recordID : v.modelName;
                bool match = false;

                switch (currentMode)
                {
                case FilterMode::Category: match = (v.category == selectedValue); break;
                case FilterMode::Model: match = (model == selectedValue); break;
                case FilterMode::Affiliation: match = (v.affiliation == selectedValue); break;
                default: break;
                }

                if (!match) continue;

                bool& toggle = vehicleToggleStates[v.recordID];
                if (buttons::Toggle(model, toggle))
                {
                    RequestVehicleToggle(v.recordID);
                }
            }
        }     SubMenu vehicleUnlockFilteredMenu{ "Filtered Vehicles", &FilteredVehicleView };


        void VehicleUnlockMainView()
        {
            using namespace feature::vehicleoptions;
            std::set<std::string> shownCategories, shownModels, shownAffiliations;

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
                const std::string& name = v.brandName.empty() ? v.recordID : v.brandName;
                if (shownModels.insert(name).second)
                {
                    if (buttons::Submenu(name.c_str(), vehicleUnlockFilteredMenu))
                    {
                        currentMode = FilterMode::Model;
                        selectedValue = name;
                    }
                }
            }

            buttons::Break("", "Filter by Affiliation");
            for (const auto& v : allVehicles)
            {
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