#pragma once

namespace view::vehicle {
    namespace vehicleunlocks {
        void VehicleUnlockMainView();
        inline render::ui::SubMenu vehicleUnlockMenu{ "Vehicle Unlock Menu", &VehicleUnlockMainView };
    }
}