#pragma once


#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/Entity.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/Puppet.hpp>

#include <RED4ext/Scripting/Natives/Generated/game/StatsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatPoolsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/VehicleSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/TeleportationFacility.hpp>
#include <RED4ext/Scripting/Natives/Generated/world/WeatherScriptInterface.hpp>


using namespace RED4ext;

namespace GameBase {
    bool TryGetGameInstance(ScriptGameInstance& outInstance);

    bool TryGetPlayerHandle(Handle<IScriptable>& outHandle);
    bool TryGetPlayerID(ent::EntityID& outID);
    bool TryGetPlayerHandleAndID(Handle<IScriptable>& outHandle, ent::EntityID& outID);
    bool TryGetVehicleHandle(RED4ext::Handle<RED4ext::IScriptable>& outHandle);
    bool TryGetVehicleHandleAndID(RED4ext::Handle<RED4ext::IScriptable>& outHandle, RED4ext::ent::EntityID& outID);
    ent::Entity* GetPlayerEntity();

    template<typename T>
    Handle<T> GetGameSystem(const char* functionName) {
        ScriptGameInstance gi;
        if (!TryGetGameInstance(gi))
            return {};

        Handle<T> sys;
        if (!ExecuteFunction("ScriptGameInstance", functionName, &sys, gi) || !sys)
            return {};

        return sys;
    }
    namespace Systems {
        Handle<StatsSystem> GetStatsSystem();
        Handle<StatPoolsSystem> GetStatPoolsSystem();
        Handle<VehicleSystem> GetVehicleSystem();
        Handle<TeleportationFacility> GetTeleportationSystem();
        Handle<WeatherSystem> GetWeatherSystem();
    }
}
