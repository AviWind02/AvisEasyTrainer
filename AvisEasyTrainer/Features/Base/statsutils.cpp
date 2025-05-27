#include "pch.h"
#include "gamebase.h"


using namespace RED4ext;
using namespace RED4ext::game;
namespace gamebase {
    namespace statsutils {

        float GetPoolValue(game::data::StatPoolType poolType) {
            ScriptGameInstance gi;
            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) ||
                !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID))
                return -1.f;

            auto statPoolSystem = gamebase::GetStatPoolsSystem();
            if (!statPoolSystem)
                return -1.f;

            auto* cls = CRTTISystem::Get()->GetClass("gameStatPoolsSystem");
            auto* fn = cls ? cls->GetFunction("GetStatPoolValue") : nullptr;
            if (!fn)
                return -1.f;

            float outValue = 0.f;
            StackArgs_t args{
                { nullptr, &playerID },
                { nullptr, &poolType },
                { nullptr, &playerHandle }
            };
            ExecuteFunction(statPoolSystem, fn, &outValue, args);
            return outValue;
        }

        float GetStatValue(game::data::StatType statType) {
            ScriptGameInstance gi;
            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) ||
                !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID))
                return -1.f;

            auto statsSystem = gamebase::GetStatsSystem();
            if (!statsSystem)
                return -1.f;

            auto* cls = CRTTISystem::Get()->GetClass("gameStatsSystem");
            auto* fn = cls ? cls->GetFunction("GetStatValue") : nullptr;
            if (!fn)
                return -1.f;

            float outValue = 0.f;
            StackArgs_t args{
                { nullptr, &playerID },
                { nullptr, &statType },
                { nullptr, &playerHandle }
            };
            ExecuteFunction(statsSystem, fn, &outValue, args);
            return outValue;
        }

        bool GetBoolStatValue(game::data::StatType statType) {
            return GetStatValue(statType) > 0.5f;
        }

        int GetIntStatValue(game::data::StatType statType) {
            return static_cast<int>(GetStatValue(statType));
        }


        bool SetPoolValue(game::data::StatPoolType poolType, float newValue, bool propagate) {
            ScriptGameInstance gi;
            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) ||
                !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID))
                return false;

            auto statPoolSystem = gamebase::GetStatPoolsSystem();
            if (!statPoolSystem)
                return false;

            auto* cls = CRTTISystem::Get()->GetClass("gameStatPoolsSystem");
            auto* fn = cls ? cls->GetFunction("RequestSettingStatPoolValue") : nullptr;
            if (!fn)
                return false;

            StackArgs_t args{
                { nullptr, &playerID },
                { nullptr, &poolType },
                { nullptr, &newValue },
                { nullptr, &playerHandle },
                { nullptr, &propagate }
            };
            ExecuteFunction(statPoolSystem, fn, nullptr, args);
            return true;
        }

        bool SetStatValue(game::data::StatType statType, float newValue, bool propagate) {
            ScriptGameInstance gi;
            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) ||
                !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID))
                return false;

            auto statsSystem = gamebase::GetStatsSystem();
            if (!statsSystem)
                return false;

            auto* cls = CRTTISystem::Get()->GetClass("gameStatsSystem");
            auto* fn = cls ? cls->GetFunction("RequestSettingStatValue") : nullptr;
            if (!fn)
                return false;

            StackArgs_t args{
                { nullptr, &playerID },
                { nullptr, &statType },
                { nullptr, &newValue },
                { nullptr, &playerHandle },
                { nullptr, &propagate }
            };
            ExecuteFunction(statsSystem, fn, nullptr, args);
            return true;
        }

        bool SetBoolStatValue(game::data::StatType statType, bool newValue, bool propagate) {
            return SetStatValue(statType, newValue ? 1.0f : 0.0f, propagate);
        }

        bool SetIntStatValue(game::data::StatType statType, int newValue, bool propagate) {
            return SetStatValue(statType, static_cast<float>(newValue), propagate);
        }
    }
}