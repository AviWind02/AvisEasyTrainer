#pragma once


#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/Entity.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatPoolsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatPoolType.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatType.hpp>

namespace gamebase {
    using namespace RED4ext;

    // Game instance access 
    bool TryGetGameInstance(ScriptGameInstance& outInstance);

    // Player access
    ent::Entity* GetPlayerEntity();
    bool TryGetPlayerHandle(Handle<IScriptable>& outHandle);
    bool TryGetPlayerID(ent::EntityID& outID);
    bool TryGetPlayerHandleAndID(Handle<IScriptable>& outHandle, ent::EntityID& outID);

    // Game system fetcher template
    template<typename T>
    Handle<T> GetGameSystem(const char* functionName);


    RED4ext::Handle<game::StatsSystem>     GetStatsSystem();
    RED4ext::Handle<game::StatPoolsSystem> GetStatPoolsSystem();


    namespace statsutils
    {
        float GetStatValue(RED4ext::game::data::StatType statType);
        int GetIntStatValue(RED4ext::game::data::StatType statType);
        bool GetBoolStatValue(RED4ext::game::data::StatType statType);

        float GetPoolValue(RED4ext::game::data::StatPoolType poolType);

        bool SetStatValue(RED4ext::game::data::StatType statType, float newValue, bool propagate = true);
        bool SetIntStatValue(RED4ext::game::data::StatType statType, int newValue, bool propagate = true);
        bool SetBoolStatValue(RED4ext::game::data::StatType statType, bool newValue, bool propagate = true);

        bool SetPoolValue(RED4ext::game::data::StatPoolType poolType, float newValue, bool propagate = true);
    }

}
