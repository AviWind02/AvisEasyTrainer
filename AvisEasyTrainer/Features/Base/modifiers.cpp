#include "pch.h"
#include "gamebase.h"
#include <unordered_map>

namespace gamebase {
    namespace modifier {

        static std::unordered_map<std::string, ModifierEntry> activeModifiers;

        RED4ext::Handle<RED4ext::game::StatModifierData> CreateStatModifier(
            RED4ext::game::data::StatType statType,
            float value,
            RED4ext::game::StatModifierType modifierType,
            const std::string& customID)
        {
            auto* rtti = RED4ext::CRTTISystem::Get();
            if (!rtti)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::CreateStatModifier] RTTI system is null");
                return nullptr;
            }

            auto* createFn = rtti->GetFunction("gameRPGManager::CreateStatModifier;gamedataStatTypegameStatModifierTypeFloat");
            if (!createFn)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::CreateStatModifier] CreateStatModifier not found");
                return nullptr;
            }

            StackArgs_t args{
                { nullptr, &statType },
                { nullptr, &modifierType },
                { nullptr, &value }
            };

            RED4ext::Handle<RED4ext::game::StatModifierData> modifierHandle;
            if (!ExecuteFunction((ScriptInstance)nullptr, createFn, &modifierHandle, args) || !modifierHandle)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::CreateStatModifier] Failed to create modifier");
                return nullptr;
            }

            activeModifiers[customID] = { RED4ext::CName(customID.c_str()), statType, modifierHandle };
            loghandler::sdk->logger->InfoF(loghandler::handle, "[Modifier] Created '%s' with type %u and value %.2f", customID.c_str(), static_cast<uint32_t>(modifierType), value);

            return modifierHandle;
        }

        bool InjectStatModifier(const RED4ext::CName& customID)
        {
            auto* rtti = RED4ext::CRTTISystem::Get();
            if (!rtti)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::InjectStatModifier] RTTI system is null");
                return false;
            }

            auto* injectFn = rtti->GetFunction("gameRPGManager::InjectStatModifier;GameInstanceGameObjectgameStatModifierData");
            if (!injectFn)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::InjectStatModifier] InjectStatModifier not found");
                return false;
            }

            auto it = activeModifiers.find(customID.ToString());
            if (it == activeModifiers.end())
                return false;

            RED4ext::ScriptGameInstance gi;
            RED4ext::Handle<RED4ext::IScriptable> playerHandle;
            RED4ext::ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) || !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID))
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::InjectStatModifier] Failed to get game instance or player handle/ID");
                return false;
            }

            StackArgs_t args{
                { nullptr, &gi },
                { nullptr, &playerHandle },
                { nullptr, &it->second.modifierHandle }
            };

            if (!ExecuteFunction((ScriptInstance)nullptr, injectFn, nullptr, args))
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::InjectStatModifier] Execution failed");
                return false;
            }
            loghandler::sdk->logger->InfoF(loghandler::handle, "[Modifier] Injected '%s'", customID);

            return true;
        }

        bool AddStatModifier(const RED4ext::CName& customID)
        {
            auto it = activeModifiers.find(customID.ToString());
            if (it == activeModifiers.end())
                return false;

            auto statsSystem = gamebase::GetStatsSystem();
            if (!statsSystem)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::AddStatModifier] statsSystem is null");
                return false;
            }



            RED4ext::Handle<RED4ext::IScriptable> playerHandle;
            RED4ext::ent::EntityID playerID;
            if (!gamebase::TryGetPlayerHandleAndID(playerHandle, playerID))
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::AddStatModifier] Failed to get player handle/ID");
                return false;
            }

            auto* cls = RED4ext::CRTTISystem::Get()->GetClass("gameStatsSystem");
            auto* fn = cls ? cls->GetFunction("AddModifier") : nullptr;
            if (!fn)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::AddStatModifier] Function AddModifier not found");
                return false;
            }

            RED4ext::game::StatsObjectID objectID(playerID);
            bool result = false;
            StackArgs_t args{
                { nullptr, &objectID },
                { nullptr, &it->second.modifierHandle }
            };
            ExecuteFunction(statsSystem, fn, &result, args);
            loghandler::sdk->logger->InfoF(loghandler::handle, "[Modifier] Injected '%s'", customID);

            if (result)
                it->second.isAdded = true;

            return result;

        }

        bool RemoveStatModifier(const RED4ext::CName& customID)
        {
            auto it = activeModifiers.find(customID.ToString());
            if (it == activeModifiers.end())
                return false;

            auto statsSystem = gamebase::GetStatsSystem();
            if (!statsSystem)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::RemoveStatModifier] statsSystem is null");
                return false;
            }

            RED4ext::Handle<RED4ext::IScriptable> playerHandle;
            RED4ext::ent::EntityID playerID;
            if (!gamebase::TryGetPlayerHandleAndID(playerHandle, playerID))
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::RemoveStatModifier] Failed to get player handle/ID");
                return false;
            }

            auto* cls = RED4ext::CRTTISystem::Get()->GetClass("gameStatsSystem");
            auto* fn = cls ? cls->GetFunction("RemoveModifier") : nullptr;
            if (!fn)
            {
                loghandler::sdk->logger->Error(loghandler::handle, "[GameBaseModifier::RemoveStatModifier] Function RemoveModifier not found");
                return false;
            }

            RED4ext::game::StatsObjectID objectID(playerID);
            bool result = false;
            StackArgs_t args{
                { nullptr, &objectID },
                { nullptr, &it->second.modifierHandle }
            };
            ExecuteFunction(statsSystem, fn, &result, args);

            if (result)
                activeModifiers.erase(it);

            loghandler::sdk->logger->InfoF(loghandler::handle, "[Modifier] Removed '%s' from player", customID);

            return result;
        }

        bool IsModifierActive(const RED4ext::CName& customID)
        {
            auto it = activeModifiers.find(customID.ToString());
            return it != activeModifiers.end() && it->second.isAdded;
        }

    } 
}
