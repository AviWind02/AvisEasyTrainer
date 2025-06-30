#pragma once

namespace view::vehicle {
    namespace vehicleunlocks {
        void VehicleUnlockMainView();
        inline UI::SubMenu vehicleUnlockMenu{ "Vehicle Unlock Menu", &VehicleUnlockMainView };
    }
}