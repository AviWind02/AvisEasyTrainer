#pragma once

#include <Base/gamebase.h>
#include <RED4ext/Scripting/Natives/Generated/game/VehicleSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/Vehicle_Record.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatType.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatData.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/StatModifierData.h>
#include <RED4ext/Scripting/Natives/Generated/game/StatsObjectID.hpp>


using namespace RED4ext;

namespace gamebase {
    namespace natives {
        namespace statmodifier {
     

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


            inline float GetStatValue(game::data::StatType statType) {
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

            inline bool AddStatModifier(Handle<game::StatModifierData> handle)
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

            inline bool RemoveStatModifier(Handle<game::StatModifierData> handle)
            {
                if (!handle)
                    return false;

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
                        loghandler::sdk->logger->InfoF(loghandler::handle, "[Modifier] Removed cached modifier ID %u", id);
                    }
                    else {
                        loghandler::sdk->logger->WarnF(loghandler::handle, "[Modifier] Failed to remove cached modifier ID %u", id);
                    }
                }

                cachedModifiers.clear();
                loghandler::sdk->logger->Info(loghandler::handle, "[Modifier] All cached modifiers removed");
            }


            inline std::vector<std::string> DumpVehicleRecordInfo(const std::string& vehicleName)
            {
                std::vector<std::string> infoLines;

                RED4ext::TweakDBID id(vehicleName);
                RED4ext::Handle<RED4ext::IScriptable> record;
                auto* tweakDB = RED4ext::TweakDB::Get();

                if (!tweakDB || !tweakDB->TryGetRecord(id, record) || !record || !record.instance)
                {
                    loghandler::sdk->logger->WarnF(loghandler::handle, "[DumpVehicle] Failed to fetch record for %s", vehicleName.c_str());
                    return infoLines;
                }

                auto* cls = record.instance->GetType();
                if (!cls)
                    return infoLines;

                infoLines.emplace_back("== Dumping Vehicle Record: " + vehicleName + " ==");

                for (auto* prop : cls->props)
                {
                    if (!prop)
                        continue;

                    std::string propName = prop->name.ToString();
                    std::string line = propName + ": ";

                    auto* type = prop->type;
                    if (!type)
                        continue;

                    const auto& typeName = type->GetName();

                    try
                    {
                        // CName
                        if (typeName == "CName")
                        {
                            auto value = prop->GetValue<RED4ext::CName>(record.instance);
                            line += value.ToString();
                        }
                        // String
                        else if (typeName == "String")
                        {
                            auto value = prop->GetValue<RED4ext::CString>(record.instance);
                            line += value.c_str();
                        }
                        // TweakDBID
                        else if (typeName == "TweakDBID")
                        {
                            auto value = prop->GetValue<RED4ext::TweakDBID>(record.instance);
                            line += value;
                        }
                        // Bool
                        else if (typeName == "Bool")
                        {
                            auto value = prop->GetValue<bool>(record.instance);
                            line += value ? "True" : "False";
                        }
                        // Float
                        else if (typeName == "Float")
                        {
                            auto value = prop->GetValue<float>(record.instance);
                            line += std::to_string(value);
                        }
                        // Int32
                        else if (typeName == "Int32")
                        {
                            auto value = prop->GetValue<int32_t>(record.instance);
                            line += std::to_string(value);
                        }
                        // array:CName
                        else if (typeName == "array:CName")
                        {
                            auto* arr = prop->GetValuePtr<RED4ext::DynArray<RED4ext::CName>>(record.instance);
                            line += "[";
                            if (arr)
                            {
                                for (size_t i = 0; i < arr->size; ++i)
                                {
                                    line += (*arr)[i].ToString();
                                    if (i + 1 < arr->size)
                                        line += ", ";
                                }
                            }
                            line += "]";
                        }
                        // array:TweakDBID
                        else if (typeName == "array:TweakDBID")
                        {
                            auto* arr = prop->GetValuePtr<RED4ext::DynArray<RED4ext::TweakDBID>>(record.instance);
                            line += "[";
                            if (arr)
                            {
                                for (size_t i = 0; i < arr->size; ++i)
                                {
                                    line += (*arr)[i];
                                    if (i + 1 < arr->size)
                                        line += ", ";
                                }
                            }
                            line += "]";
                        }
                        else
                        {
                            line += "(Unrecognized Type: " + typeName;
                        }
                    }
                    catch (...)
                    {
                        line += "(Error reading value)";
                    }

                    infoLines.push_back(line);
                }

                return infoLines;
            }


            inline bool UnlockPlayerVehicle(CString vehicleName)
            {
                auto vehicleSystem = gamebase::GetGameSystem<game::VehicleSystem>("GetVehicleSystem");
                if (!vehicleSystem)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[UnlockPlayerVehicle] Failed to get VehicleSystem");
                    return false;
                }

                auto* rtti = RED4ext::CRTTISystem::Get();
                if (!rtti)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[UnlockPlayerVehicle] RTTI system is null");
                    return false;
                }


                auto* fn = rtti->GetClass("gameVehicleSystem")->GetFunction("EnablePlayerVehicle");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[UnlockPlayerVehicle] Function not found");
                    return false;
                }

                bool enable = true;
                bool despawnIfDisabling = false;

                StackArgs_t args{
                    { nullptr, &vehicleName },
                    { nullptr, &enable },
                    { nullptr, &despawnIfDisabling }
                };

                bool res = false;
                if (!ExecuteFunction(vehicleSystem, fn, &res, args))
                {
                    loghandler::sdk->logger->ErrorF(loghandler::handle, "[UnlockPlayerVehicle] Failed to unlock vehicle: %s", vehicleName.c_str());
                    return false;
                }

               
                loghandler::sdk->logger->InfoF(loghandler::handle, "[UnlockPlayerVehicle] VisualTags for %s: %s", vehicleName.c_str(), DumpVehicleRecordInfo(vehicleName.c_str()));


                

                loghandler::sdk->logger->InfoF(loghandler::handle, "[VehicleSystem] Unlocked vehicle: %s", vehicleName.c_str());
                return res;
            }

        }
    }
}

/*
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

*/