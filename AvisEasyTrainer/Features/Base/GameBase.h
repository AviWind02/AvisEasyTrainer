#pragma once


#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/Entity.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatPoolsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatPoolType.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatType.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatData.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatModifierData.h>

#include <RED4ext/Scripting/Natives/Generated/game/StatsObjectID.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/RPGManager.hpp>

namespace gamebase {
    using namespace RED4ext;

    // Game instance access 
    bool TryGetGameInstance(ScriptGameInstance& outInstance);

    // Player access

    bool TryGetPlayerHandle(Handle<IScriptable>& outHandle);
    bool TryGetPlayerID(ent::EntityID& outID);
    bool TryGetPlayerHandleAndID(Handle<IScriptable>& outHandle, ent::EntityID& outID);
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



    RED4ext::Handle<game::StatsSystem>     GetStatsSystem();
    RED4ext::Handle<game::StatPoolsSystem> GetStatPoolsSystem();

    namespace modifier {

        struct ModifierEntry
        {
            RED4ext::CName customID;
            RED4ext::game::data::StatType statType;
            RED4ext::Handle<RED4ext::game::StatModifierData> modifierHandle;
            bool isAdded = false;
        };

        RED4ext::Handle<RED4ext::game::StatModifierData> CreateStatModifier(
            RED4ext::game::data::StatType statType,
            float value,
            RED4ext::game::StatModifierType modifierType,
            const std::string& customID);

        bool AddStatModifier(const RED4ext::CName& customID);
        bool RemoveStatModifier(const RED4ext::CName& customID);
        bool InjectStatModifier(const RED4ext::CName& customID);
        bool IsModifierActive(const RED4ext::CName& customID);

    }

    namespace statsutils
    {
        float GetStatValue(RED4ext::game::data::StatType statType);
        bool InjectStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType = game::StatModifierType::Additive);
        float GetPoolValue(RED4ext::game::data::StatPoolType poolType);
        bool SetPoolValue(RED4ext::game::data::StatPoolType poolType, float newValue, bool propagate = true);
    }

}
