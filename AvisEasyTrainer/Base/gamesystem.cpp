#include "pch.h"
#include "GameBase.h"

namespace GameBase::Systems {

    Handle<StatsSystem> GetStatsSystem() {
        return GetGameSystem<StatsSystem>("GetStatsSystem");
    }

    Handle<StatPoolsSystem> GetStatPoolsSystem() {
        return GetGameSystem<StatPoolsSystem>("GetStatPoolsSystem");
    }

    Handle<VehicleSystem> GetVehicleSystem() {
        return GetGameSystem<VehicleSystem>("GetVehicleSystem");
    }
    
    Handle<TeleportationFacility> GetTeleportationSystem() {
        return GetGameSystem<TeleportationFacility>("GetTeleportationFacility");
    }

    Handle<WeatherSystem> GetWeatherSystem() {
        return GetGameSystem<WeatherSystem>("GetWeatherSystem");
    }
}
