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



    Handle<game::StatsSystem>     GetStatsSystem();
    Handle<game::StatPoolsSystem> GetStatPoolsSystem();

    namespace modifier {

        struct ModifierEntry
        {
            CName customID;
            game::data::StatType statType;
            Handle<game::StatModifierData> modifierHandle;
            bool isAdded = false;
        };

        Handle<game::StatModifierData> CreateStatModifier(
            game::data::StatType statType,
            float value,
            game::StatModifierType modifierType,
            uint32_t* outID = nullptr);

        bool InjectStatModifier(Handle<game::StatModifierData> handle);
        bool AddStatModifier(Handle<game::StatModifierData> handle);
        bool RemoveStatModifier(Handle<game::StatModifierData> handle);

        bool InjectStatModifier(
            game::data::StatType statType,
            float value,
            game::StatModifierType modifierType);

        bool AddStatModifier(
            game::data::StatType statType,
            float value,
            game::StatModifierType modifierType);

        void RemoveAllCachedModifiers();
    }

    namespace statsutils
    {
        float GetStatValue(game::data::StatType statType);
        bool InjectStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType = game::StatModifierType::Additive);
        float GetPoolValue(game::data::StatPoolType poolType);
        bool SetPoolValue(game::data::StatPoolType poolType, float newValue, bool propagate = true);
    }

}
