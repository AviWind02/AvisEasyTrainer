#include "pch.h"
#include "gamebase.h"
#include <unordered_set>

#include <RED4ext/Scripting/Natives/Generated/game/StatModifierDetailedData.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/RPGManager.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/CombinedStatOperation.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatObjectsRelation.hpp>

using namespace RED4ext;
using namespace RED4ext::game;

namespace gamebase {
    namespace statsutils {

        float GetPoolValue(game::data::StatPoolType poolType) {
            //loghandler::sdk->logger->InfoF(loghandler::handle, "[GetPoolValue] Called with poolType: %d", static_cast<int>(poolType));
            ScriptGameInstance gi;
            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) || !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID)) {
                loghandler::sdk->logger->Error(loghandler::handle, "[GetPoolValue] Failed to get game instance or player handle/ID");
                return -1.f;
            }
           // loghandler::sdk->logger->InfoF(loghandler::handle, "[GetPoolValue] Obtained GameInstance and PlayerID: %u", playerID);

            auto statPoolSystem = gamebase::GetStatPoolsSystem();
            if (!statPoolSystem) {
                loghandler::sdk->logger->Error(loghandler::handle, "[GetPoolValue] statPoolSystem is null");
                return -1.f;
            }

            auto* cls = CRTTISystem::Get()->GetClass("gameStatPoolsSystem");
            auto* fn = cls ? cls->GetFunction("GetStatPoolValue") : nullptr;
            if (!fn) {
                loghandler::sdk->logger->Error(loghandler::handle, "[GetPoolValue] Function GetStatPoolValue not found");
                return -1.f;
            }

            float outValue = 0.f;
            StackArgs_t args{
                { nullptr, &playerID },
                { nullptr, &poolType },
                { nullptr, &playerHandle }
            };
            ExecuteFunction(statPoolSystem, fn, &outValue, args);
            //loghandler::sdk->logger->InfoF(loghandler::handle, "[GetPoolValue] Result: %f", outValue);
            return outValue;
        }

        float GetStatValue(game::data::StatType statType) {
            //loghandler::sdk->logger->InfoF(loghandler::handle, "[GetStatValue] Called with statType: %d", static_cast<int>(statType));
            ScriptGameInstance gi;
            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) || !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID)) {
                loghandler::sdk->logger->Error(loghandler::handle, "[GetStatValue] Failed to get game instance or player handle/ID");
                return -1.f;
            }
            //loghandler::sdk->logger->InfoF(loghandler::handle, "[GetStatValue] Obtained GameInstance and PlayerID: %u", playerID);

            auto statsSystem = gamebase::GetStatsSystem();
            if (!statsSystem) {
                loghandler::sdk->logger->Error(loghandler::handle, "[GetStatValue] statsSystem is null");
                return -1.f;
            }

            auto* cls = CRTTISystem::Get()->GetClass("gameStatsSystem");
            auto* fn = cls ? cls->GetFunction("GetStatValue") : nullptr;
            if (!fn) {
                loghandler::sdk->logger->Error(loghandler::handle, "[GetStatValue] Function GetStatValue not found");
                return -1.f;
            }

            float outValue = 0.f;
            StackArgs_t args{
                { nullptr, &playerID },
                { nullptr, &statType },
                { nullptr, &playerHandle }
            };
            ExecuteFunction(statsSystem, fn, &outValue, args);
            //loghandler::sdk->logger->InfoF(loghandler::handle, "[GetStatValue] Result: %f", outValue);
            return outValue;
        }

 
        bool SetPoolValue(game::data::StatPoolType poolType, float newValue, bool propagate)     {
            //loghandler::sdk->logger->InfoF(loghandler::handle, "[SetPoolValue] Called with poolType: %d, newValue: %f, propagate: %s", static_cast<int>(poolType), newValue, propagate ? "true" : "false");
            ScriptGameInstance gi;
            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) || !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID)) {
                loghandler::sdk->logger->Error(loghandler::handle, "[SetPoolValue] Failed to get game instance or player handle/ID");
                return false;
            }

            auto statPoolSystem = gamebase::GetStatPoolsSystem();
            if (!statPoolSystem) {
                loghandler::sdk->logger->Error(loghandler::handle, "[SetPoolValue] statPoolSystem is null");
                return false;
            }

            auto* cls = CRTTISystem::Get()->GetClass("gameStatPoolsSystem");
            auto* fn = cls ? cls->GetFunction("RequestSettingStatPoolValue") : nullptr;
            if (!fn) {
                loghandler::sdk->logger->Error(loghandler::handle, "[SetPoolValue] Function RequestSettingStatPoolValue not found");
                return false;
            }

            StackArgs_t args{
                { nullptr, &playerID },
                { nullptr, &poolType },
                { nullptr, &newValue },
                { nullptr, &playerHandle },
                { nullptr, &propagate }
            };
            ExecuteFunction(statPoolSystem, fn, nullptr, args);
            //loghandler::sdk->logger->Info(loghandler::handle, "[SetPoolValue] Executed successfully");
            return true;
        }



        bool InjectStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType)
        {
            auto* rtti = RED4ext::CRTTISystem::Get();
            if (!rtti)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] RTTI system is null");
                return false;
            }

            auto* createFn = rtti->GetFunction("gameRPGManager::CreateStatModifier;gamedataStatTypegameStatModifierTypeFloat");
            if (!createFn)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] CreateStatModifier not found");
                return false;
            }

            StackArgs_t createArgs{
                { nullptr, &statType },
                { nullptr, &modifierType },
                { nullptr, &value }
            };

            RED4ext::Handle<game::StatModifierData> modifierHandle;
            if (!ExecuteFunction((ScriptInstance)nullptr, createFn, &modifierHandle, createArgs) || !modifierHandle)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] Failed to create modifier");
                return false;
            }

            auto* injectFn = rtti->GetFunction("gameRPGManager::InjectStatModifier;GameInstanceGameObjectgameStatModifierData");
            if (!injectFn)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] InjectStatModifier not found");
                return false;
            }

            RED4ext::ScriptGameInstance gi;
            if (!gamebase::TryGetGameInstance(gi))
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] Failed to get GameInstance");
                return false;
            }

            RED4ext::Handle<RED4ext::IScriptable> playerHandle;
            RED4ext::ent::EntityID playerID;
            if (!gamebase::TryGetPlayerHandleAndID(playerHandle, playerID))
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] Failed to get player handle");
                return false;
            }

            StackArgs_t injectArgs{
                { nullptr, &gi },
                { nullptr, &playerHandle },
                { nullptr, &modifierHandle }
            };

            if (!ExecuteFunction((ScriptInstance)nullptr, injectFn, nullptr, injectArgs))
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] Failed to inject modifier");
                return false;
            }

            return true;
        }

       /* bool CheckRevealPlayerPosition(game::NPCPuppet* ownerPuppet, const ent::EntityID& playerID, bool isPrevention)
        {
            auto* rtti = RED4ext::CRTTISystem::Get();
            if (!rtti)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[CheckReveal] RTTI system is null");
                return false;
            }

            auto* revealFn = rtti->GetFunction("NPCPuppet::RevealPlayerPositionIfNeeded;ScriptedPuppetEntityIDBool");
            if (!revealFn)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[CheckReveal] RevealPlayerPositionIfNeeded not found");
                return false;
            }

            RED4ext::Handle<RED4ext::IScriptable> ownerHandle = reinterpret_cast<RED4ext::IScriptable*>(ownerPuppet);

            StackArgs_t args{
                { nullptr, &ownerHandle },
                { nullptr, &playerID },
                { nullptr, &isPrevention }
            };

            bool result = false;
            if (!ExecuteFunction(nullptr, revealFn, &result, args))
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[CheckReveal] Failed to execute RevealPlayerPositionIfNeeded");
                return false;
            }

            loghandler::sdk->logger->InfoF(loghandler::handle, "[CheckReveal] RevealPlayerPositionIfNeeded returned: %s", result ? "true" : "false");

            return result;
        }*/

    } 
} 