#include "pch.h"
#include "gamebase.h"
#include <unordered_map>

namespace gamebase {
    namespace modifier {

        struct CachedModifier {
            uint32_t id;
            Handle<game::StatModifierData> handle;
        };

        static std::unordered_map<uint32_t, CachedModifier> cachedModifiers;
        static uint32_t nextModifierID = 1;

        Handle<game::StatModifierData> CreateStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType, uint32_t* outID)
        {
            auto* rtti = CRTTISystem::Get();
            if (!rtti) {
                loghandler::sdk->logger->Error(loghandler::handle, "[CreateStatModifier] RTTI system is null");
                return nullptr;
            }

            auto* createFn = rtti->GetFunction("gameRPGManager::CreateStatModifier;gamedataStatTypegameStatModifierTypeFloat");
            if (!createFn) {
                loghandler::sdk->logger->Error(loghandler::handle, "[CreateStatModifier] Function not found");
                return nullptr;
            }

            StackArgs_t args{
                { nullptr, &statType },
                { nullptr, &modifierType },
                { nullptr, &value }
            };

            Handle<game::StatModifierData> modifierHandle;
            if (!ExecuteFunction((ScriptInstance)nullptr, createFn, &modifierHandle, args) || !modifierHandle) {
                loghandler::sdk->logger->Error(loghandler::handle, "[CreateStatModifier] Execution failed");
                return nullptr;
            }

            if (outID) {
                uint32_t id = nextModifierID++;
                cachedModifiers[id] = { id, modifierHandle };
                *outID = id;
            }

            loghandler::sdk->logger->InfoF(loghandler::handle, "[Modifier] Created stat modifier for type %u with value %.2f", static_cast<uint32_t>(modifierType), value);
            return modifierHandle;
        }

        bool InjectStatModifier(Handle<game::StatModifierData> handle)
        {
            auto* rtti = CRTTISystem::Get();
            if (!rtti) {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] RTTI system is null");
                return false;
            }

            auto* injectFn = rtti->GetFunction("gameRPGManager::InjectStatModifier;GameInstanceGameObjectgameStatModifierData");
            if (!injectFn) {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] Function not found");
                return false;
            }

            ScriptGameInstance gi;
            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;

            if (!gamebase::TryGetGameInstance(gi) || !gamebase::TryGetPlayerHandleAndID(playerHandle, playerID)) {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] Failed to get game instance or player handle");
                return false;
            }

            StackArgs_t args{
                { nullptr, &gi },
                { nullptr, &playerHandle },
                { nullptr, &handle }
            };

            if (!ExecuteFunction((ScriptInstance)nullptr, injectFn, nullptr, args)) {
                loghandler::sdk->logger->Error(loghandler::handle, "[InjectStatModifier] Execution failed");
                return false;
            }

            loghandler::sdk->logger->Info(loghandler::handle, "[Modifier] Injected modifier successfully");
            return true;
        }

        bool AddStatModifier(Handle<game::StatModifierData> handle)
        {
            auto statsSystem = gamebase::GetStatsSystem();
            if (!statsSystem) {
                loghandler::sdk->logger->Error(loghandler::handle, "[AddStatModifier] statsSystem is null");
                return false;
            }

            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;
            if (!gamebase::TryGetPlayerHandleAndID(playerHandle, playerID)) {
                loghandler::sdk->logger->Error(loghandler::handle, "[AddStatModifier] Failed to get player handle/ID");
                return false;
            }

            auto* cls = CRTTISystem::Get()->GetClass("gameStatsSystem");
            auto* fn = cls ? cls->GetFunction("AddModifier") : nullptr;
            if (!fn) {
                loghandler::sdk->logger->Error(loghandler::handle, "[AddStatModifier] Function AddModifier not found");
                return false;
            }

            game::StatsObjectID objectID(playerID);
            bool result = false;
            StackArgs_t args{
                { nullptr, &objectID },
                { nullptr, &handle }
            };

            ExecuteFunction(statsSystem, fn, &result, args);

            if (result)
                loghandler::sdk->logger->Info(loghandler::handle, "[Modifier] Added modifier to player");
            else
                loghandler::sdk->logger->Warn(loghandler::handle, "[Modifier] AddModifier returned false");

            return result;
        }

        bool RemoveStatModifier(Handle<game::StatModifierData> handle)
        {
            auto statsSystem = gamebase::GetStatsSystem();
            if (!statsSystem) {
                loghandler::sdk->logger->Error(loghandler::handle, "[RemoveStatModifier] statsSystem is null");
                return false;
            }

            Handle<IScriptable> playerHandle;
            ent::EntityID playerID;
            if (!gamebase::TryGetPlayerHandleAndID(playerHandle, playerID)) {
                loghandler::sdk->logger->Error(loghandler::handle, "[RemoveStatModifier] Failed to get player handle/ID");
                return false;
            }

            auto* cls = CRTTISystem::Get()->GetClass("gameStatsSystem");
            auto* fn = cls ? cls->GetFunction("RemoveModifier") : nullptr;
            if (!fn) {
                loghandler::sdk->logger->Error(loghandler::handle, "[RemoveStatModifier] Function RemoveModifier not found");
                return false;
            }

            game::StatsObjectID objectID(playerID);
            bool result = false;
            StackArgs_t args{
                { nullptr, &objectID },
                { nullptr, &handle }
            };

            ExecuteFunction(statsSystem, fn, &result, args);

            if (result)
                loghandler::sdk->logger->Info(loghandler::handle, "[Modifier] Removed modifier from player");
            else
                loghandler::sdk->logger->Warn(loghandler::handle, "[Modifier] RemoveModifier returned false");

            return result;
        }


        bool InjectStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType)
        {
            auto handle = CreateStatModifier(statType, value, modifierType);
            if (!handle)
                return false;

            return InjectStatModifier(handle);
        }

        bool AddStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType)
        {
            auto handle = CreateStatModifier(statType, value, modifierType);
            if (!handle)
                return false;

            return AddStatModifier(handle);
        }

        void RemoveAllCachedModifiers()
        {
            for (const auto& [id, entry] : cachedModifiers)
            {
                if (RemoveStatModifier(entry.handle)) {
                    loghandler::sdk->logger->InfoF(loghandler::handle, "[Modifier] Removed cached modifier ID %u", id);
                }
                else {
                    loghandler::sdk->logger->WarnF(loghandler::handle, "[Modifier] Failed to remove cached modifier ID %u", id);
                }
            }

            cachedModifiers.clear();
            loghandler::sdk->logger->Info(loghandler::handle, "[Modifier] All cached modifiers removed");
        }
    }
}
