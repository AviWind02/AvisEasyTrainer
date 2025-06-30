#include "pch.h"
#include "Base/gamebase.h"
#include "Base/Natives/Vehicle.h"

#include "vehiclefeature.h"

namespace Feature {
    namespace VehicleOptions {
   
        std::unordered_map<std::string, bool> vehicleToggleStates;
        bool tickVehicleToggleNow = false;
        std::string selectedVehicleID;




        void RequestVehicleToggle(const std::string& vehicleID)
        {
            selectedVehicleID = vehicleID;
            tickVehicleToggleNow = true;
        }

        void UpdateVehicleNames()
        {
            for (auto& vehicle : allVehicles)
            {
                const char* id = vehicle.recordID.empty() ? "<empty_id>" : vehicle.recordID.c_str();
                std::string localizedName = GameBase::Natives::Vehicle::LocalizeVehicleDisplayName(vehicle.recordID);
                vehicle.modelName = localizedName.empty() ? "<unnamed>" : localizedName;

                loghandler::sdk->logger->InfoF(loghandler::handle,
                    "[UpdateVehicleNames] %s resolved as '%s'",
                    id, vehicle.modelName.c_str());
            }
        }

        void UpdateVehicleUnlockStates()
        {
            for (const auto& vehicle : allVehicles)
            {
                const char* id = vehicle.recordID.empty() ? "<empty_id>" : vehicle.recordID.c_str();
                RED4ext::TweakDBID tweakID(id);

                bool isUnlocked = GameBase::Natives::Vehicle::IsVehicleUnlocked(tweakID);
                vehicleToggleStates[vehicle.recordID] = isUnlocked;

                loghandler::sdk->logger->InfoF(loghandler::handle,
                    "[UpdateVehicleUnlockStates] %s (%s) is %s",
                    vehicle.modelName.empty() ? "<unnamed>" : vehicle.modelName.c_str(),
                    id,
                    isUnlocked ? "UNLOCKED" : "LOCKED");
            }
        }

        bool hasInitialized = true;
        void Tick()
        {
            if (!hasInitialized)
            {
                UpdateVehicleUnlockStates();
                hasInitialized = true;
            }

            if (!tickVehicleToggleNow)
                return;

            auto it = std::find_if(allVehicles.begin(), allVehicles.end(),
                [](const VehicleMetadata& v) { return v.recordID == selectedVehicleID; });

            if (it != allVehicles.end())
            {
                const RED4ext::CString vehicleName(it->recordID.c_str());
                RED4ext::TweakDBID tweakID(vehicleName.c_str());
                bool isCurrentlyUnlocked = GameBase::Natives::Vehicle::IsVehicleUnlocked(tweakID);

                bool desiredState = !isCurrentlyUnlocked;

                GameBase::Natives::Vehicle::SetPlayerVehicleState(vehicleName, desiredState);
                vehicleToggleStates[it->recordID] = desiredState;
            }

            tickVehicleToggleNow = false;
        }

    }
}