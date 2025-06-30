#pragma once
#include "Base/GameBase.h"

#include <RED4ext/Scripting/Natives/Generated/game/data/StatType.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatData.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatModifierData.h>
#include <RED4ext/Scripting/Natives/Generated/game/StatsObjectID.hpp>

namespace GameBase {
    namespace Natives {
        namespace StatModifier {
     

            struct ModifierEntry
            {
                CName customID;
                game::data::StatType statType;
                Handle<game::StatModifierData> modifierHandle;
                bool isAdded = false;
            };


            struct CachedModifier {
                uint32_t id;
                Handle<game::StatModifierData> handle;
            };

            static std::unordered_map<uint32_t, CachedModifier> cachedModifiers;
            static uint32_t nextModifierID = 1;

            inline Handle<game::StatModifierData> CreateStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType, uint32_t* outID = nullptr)
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


            inline float GetStatValue(game::data::StatType statType)
            {
                Handle<IScriptable> playerHandle{};
                ent::EntityID playerID{};

                if (!TryGetPlayerHandleAndID(playerHandle, playerID))
                    return -1.f;

                auto statsSystem = Systems::GetStatsSystem();
                if (!statsSystem) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[GetStatValue] StatsSystem is null.");
                    return -1.f;
                }

                auto* cls = CRTTISystem::Get()->GetClass("gameStatsSystem");
                auto* fn = cls ? cls->GetFunction("GetStatValue") : nullptr;
                if (!fn) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[GetStatValue] Function not found.");
                    return -1.f;
                }

                float outValue = 0.f;
                StackArgs_t args{
                    { nullptr, &playerID },
                    { nullptr, &statType },
                    { nullptr, &playerHandle }
                };

                if (!ExecuteFunction(statsSystem, fn, &outValue, args)) {
                    loghandler::sdk->logger->ErrorF(loghandler::handle,
                        "[GetStatValue] Execution failed for stat.");
                    return -1.f;
                }

                return outValue;
            }


            // InjectStatModifier is functionally the same as AddModifier. 
            // Internally, it just calls AddModifier (per PsiberX's StatModifier info), so this function is 
            // redundant I guess..
            inline bool InjectStatModifier(Handle<game::StatModifierData> handle)
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

                if (!TryGetGameInstance(gi) || !TryGetPlayerHandle(playerHandle)) {
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

            inline bool AddStatModifier(Handle<game::StatModifierData> handle)
            {
                auto statsSystem = Systems::GetStatsSystem();
                if (!statsSystem) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[AddStatModifier] statsSystem is null");
                    return false;
                }

                ent::EntityID playerID;
                if (!TryGetPlayerID(playerID)) {
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

                if (!ExecuteFunction(statsSystem, fn, &result, args)) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[AddStatModifier] Execution failed");
                    return false;
                }

                if (!result)
                    loghandler::sdk->logger->Warn(loghandler::handle, "[AddStatModifier] AddModifier returned false");
                return result;
            }

            inline bool RemoveStatModifier(Handle<game::StatModifierData> handle)
            {
                if (!handle)
                    return false;

                auto statsSystem = Systems::GetStatsSystem();
                if (!statsSystem) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[RemoveStatModifier] statsSystem is null");
                    return false;
                }

                Handle<IScriptable> playerHandle;
                ent::EntityID playerID;
                if (!TryGetPlayerHandleAndID(playerHandle, playerID)) {
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

                if (!ExecuteFunction(statsSystem, fn, &result, args)) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[RemoveStatModifier] Execution failed");
                    return false;
                }

                if (!result)
                    loghandler::sdk->logger->Warn(loghandler::handle, "[RemoveStatModifier] RemoveModifier returned false");
                return result;
            }


            inline bool InjectStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType, Handle<game::StatModifierData>& outHandle)
            {
                outHandle = CreateStatModifier(statType, value, modifierType);
                if (!outHandle)
                    return false;

                return InjectStatModifier(outHandle);
            }

            inline bool AddStatModifier(game::data::StatType statType, float value, game::StatModifierType modifierType, Handle<game::StatModifierData>& outHandle)
            {
                outHandle = CreateStatModifier(statType, value, modifierType);
                if (!outHandle)
                    return false;

                return AddStatModifier(outHandle);
            }

            inline void RemoveAllCachedModifiers()
            {
                for (const auto& [id, entry] : cachedModifiers)
                {
                    if (RemoveStatModifier(entry.handle)) {
                        loghandler::sdk->logger->InfoF(loghandler::handle, "[RemoveAllCachedModifiers] Removed cached modifier ID %u", id);
                    }
                    else {
                        loghandler::sdk->logger->WarnF(loghandler::handle, "[RemoveAllCachedModifiers] Failed to remove cached modifier ID %u", id);
                    }
                }

                cachedModifiers.clear();
                loghandler::sdk->logger->Info(loghandler::handle, "[RemoveAllCachedModifiers] All cached modifiers removed");
            }


        }
    }
}