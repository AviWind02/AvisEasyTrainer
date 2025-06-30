#pragma once

namespace View::Vehicle {
    namespace Vehicleunlocks {
        void VehicleUnlockMainView();
        inline UI::SubMenu vehicleUnlockMenu{ "Vehicle Unlock Menu", &VehicleUnlockMainView };
    }
}