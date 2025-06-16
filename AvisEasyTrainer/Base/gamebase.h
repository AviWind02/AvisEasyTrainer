#pragma once


#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/Entity.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatPoolsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/Puppet.hpp>

using namespace RED4ext;

namespace gamebase {
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

    Handle<game::StatsSystem>     GetStatsSystem();
    Handle<game::StatPoolsSystem> GetStatPoolsSystem();
}
