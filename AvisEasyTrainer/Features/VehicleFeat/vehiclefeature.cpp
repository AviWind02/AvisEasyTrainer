#include "pch.h"
#include "Base/gamebase.h"
#include "Base/Natives/vehicleclass.h"

#include "vehiclefeature.h"

namespace feature {
    namespace vehicleoptions {
   
        std::unordered_map<std::string, bool> vehicleToggleStates;
        bool tickVehicleToggleNow = false;
        std::string selectedVehicleID;




        void RequestVehicleToggle(const std::string& vehicleID)
        {
            selectedVehicleID = vehicleID;
            tickVehicleToggleNow = true;
        }
        bool hasInitialized = true;
        void Tick()
        {
            if (!hasInitialized)
            {
                for (auto& vehicle : allVehicles)
                {
                    const char* id = vehicle.recordID.empty() ? "<empty_id>" : vehicle.recordID.c_str();
                    RED4ext::TweakDBID tweakID(id);

                    std::string localizedName = gamebase::natives::vehicle::LocalizeVehicleDisplayName(vehicle.recordID);
                    vehicle.modelName = (localizedName.empty() ? "<unnamed>" : localizedName);

                    bool isUnlocked = gamebase::natives::vehicle::IsVehicleUnlocked(tweakID);
                    vehicleToggleStates[vehicle.recordID] = isUnlocked;

                    loghandler::sdk->logger->InfoF(loghandler::handle,
                        "[InitVehicleToggleStates] %s (%s) is %s",
                        vehicle.modelName,
                        id,
                        isUnlocked ? "UNLOCKED" : "LOCKED");
                }

                hasInitialized = true;
            }

            if (!tickVehicleToggleNow)
                return;

            auto it = std::find_if(allVehicles.begin(), allVehicles.end(),
                [](const Vehicle& v) { return v.recordID == selectedVehicleID; });

            if (it != allVehicles.end())
            {
                const RED4ext::CString vehicleName(it->recordID.c_str());
                RED4ext::TweakDBID tweakID(vehicleName.c_str());
                bool isCurrentlyUnlocked = gamebase::natives::vehicle::IsVehicleUnlocked(tweakID);

                bool desiredState = !isCurrentlyUnlocked;

                gamebase::natives::vehicle::SetPlayerVehicleState(vehicleName, desiredState);
                vehicleToggleStates[it->recordID] = desiredState;
            }

            tickVehicleToggleNow = false;
        }

    }
}