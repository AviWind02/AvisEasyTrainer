#pragma once
#include "Base/GameBase.h"

#include <RED4ext/Scripting/Natives/Generated/game/data/Vehicle_Record.hpp>

namespace GameBase {
    namespace Natives {
        namespace Vehicle {

            inline bool IsVehicleUnlocked(RED4ext::TweakDBID id)
            {
                loghandler::sdk->logger->Info(loghandler::handle, "[IsVehicleUnlocked] Called.");

                loghandler::sdk->logger->InfoF(
                    loghandler::handle,
                    "[IsVehicleUnlocked] TweakDBID name hash: 0x%08X, name length: %u",
                    id.name.hash, id.name.length
                );

                auto vehicleSystem = GetGameSystem<game::VehicleSystem>("GetVehicleSystem");
                if (!vehicleSystem)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[IsVehicleUnlocked] Failed to get VehicleSystem");
                    return false;
                }

                auto* rtti = RED4ext::CRTTISystem::Get();
                if (!rtti)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[IsVehicleUnlocked] CRTTISystem is null");
                    return false;
                }

                auto* cls = rtti->GetClass("gameVehicleSystem");
                if (!cls)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[IsVehicleUnlocked] Class 'gameVehicleSystem' not found");
                    return false;
                }

                auto* fn = cls->GetFunction("IsVehiclePlayerUnlocked");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[IsVehicleUnlocked] Function 'IsVehiclePlayerUnlocked' not found");
                    return false;
                }

                bool result = false;
                StackArgs_t args{ { nullptr, &id } };

                if (!ExecuteFunction(vehicleSystem, fn, &result, args))
                {
                    loghandler::sdk->logger->ErrorF(
                        loghandler::handle,
                        "[IsVehicleUnlocked] ExecuteFunction failed for TweakDBID: 0x%08X",
                        id.name.hash
                    );
                    return false;
                }

                loghandler::sdk->logger->InfoF(
                    loghandler::handle,
                    "[IsVehicleUnlocked] Vehicle unlock check result for 0x%08X: %s",
                    id.name.hash, result ? "true" : "false"
                );
                return result;
            }

            inline bool SetPlayerVehicleState(CString vehicleName, bool enable = true)
            {
                TweakDBID recordID(vehicleName.c_str());
                bool currentlyUnlocked = IsVehicleUnlocked(recordID);

                if (currentlyUnlocked == enable)
                {
                    loghandler::sdk->logger->InfoF(loghandler::handle,
                        "[SetPlayerVehicleState] Vehicle %s is already in desired state (%s)",
                        vehicleName.c_str(), enable ? "unlocked" : "disabled");
                    return true;
                }

                auto vehicleSystem = Systems::GetVehicleSystem();
                if (!vehicleSystem)
                    return false;

                auto* fn = RED4ext::CRTTISystem::Get()->GetClass("gameVehicleSystem")->GetFunction("EnablePlayerVehicle");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerVehicleState] Function not found");
                    return false;
                }

                bool despawn = !enable;
                StackArgs_t args{ { nullptr, &vehicleName }, { nullptr, &enable }, { nullptr, &despawn } };

                bool result = false;
                if (!ExecuteFunction(vehicleSystem, fn, &result, args))
                {
                    loghandler::sdk->logger->ErrorF(loghandler::handle, "[SetPlayerVehicleState] Failed to %s vehicle: %s",
                        enable ? "unlock" : "disable", vehicleName.c_str());
                    return false;
                }


                loghandler::sdk->logger->InfoF(loghandler::handle, "[VehicleSystem] %s vehicle: %s",
                    enable ? "Unlocked" : "Disabled", vehicleName.c_str());
                return result;
            }

            inline bool UnlockPlayerVehicle(CString vehicleName)
            {
                return SetPlayerVehicleState(vehicleName, true);
            }

            inline bool DisablePlayerVehicle(CString vehicleName)
            {
                return SetPlayerVehicleState(vehicleName, false);
            }

            inline void EnableAllVehicles()
            {
                auto vehicleSystem = Systems::GetVehicleSystem();
                if (!vehicleSystem)
                    return;

                auto* fn = RED4ext::CRTTISystem::Get()->GetClass("gameVehicleSystem")->GetFunction("EnableAllPlayerVehicles");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[EnableAllVehicles] Function not found");
                    return;
                }

                if (!ExecuteFunction(vehicleSystem, fn, nullptr))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[EnableAllVehicles] Execution failed");
                    return;
                }

                loghandler::sdk->logger->Info(loghandler::handle, "[VehicleSystem] Enabled all player vehicles");
            }
            inline void InjectSingleVehicleIntoTweakDB(const std::string& recordName)
            {
                auto* tdb = RED4ext::TweakDB::Get();
                if (!tdb)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[InjectVehicle] TweakDB not available");
                    return;
                }

                TweakDBID vehicleListID("Vehicle.vehicle_list.list");
                DynArray<TweakDBID> vehicleList;

                if (!tdb->TryGetValue(vehicleListID, vehicleList))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[InjectVehicle] Failed to read vehicle list");
                    return;
                }

                TweakDBID newVehicleID(recordName.c_str());
                if (std::find(vehicleList.begin(), vehicleList.end(), newVehicleID) != vehicleList.end())
                {
                    loghandler::sdk->logger->InfoF(loghandler::handle, "[InjectVehicle] Vehicle %s already in list", recordName.c_str());
                    return;
                }

                vehicleList.PushBack(newVehicleID);

                RED4ext::CStackType stack;
                stack.type = RED4ext::CRTTISystem::Get()->GetType("array:TweakDBID");
                stack.value = &vehicleList;

                int32_t offset = tdb->CreateFlatValue(stack);
                if (offset == -1)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[InjectVehicle] Failed to create new flat value");
                    return;
                }

                vehicleListID.SetTDBOffset(offset);

                tdb->RemoveFlat(vehicleListID); 
                if (!tdb->AddFlat(vehicleListID))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[InjectVehicle] AddFlat failed");
                    return;
                }

            /*  TweakDBID parentRecordID("Vehicle.vehicle_list");
                if (!tdb->UpdateRecord(parentRecordID))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[InjectVehicle] UpdateRecord on parent failed");
                    return;
                }*/

                loghandler::sdk->logger->InfoF(loghandler::handle, "[InjectVehicle] Added %s to vehicle list", recordName.c_str());
            }

            inline std::string GetLocalizedTextByKey(const RED4ext::CName& key)
            {
                using namespace RED4ext;

                auto* rtti = CRTTISystem::Get();
                if (!rtti)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[Localization] RTTI system is null");
                    return "<RTTI nullptr>";
                }

                auto* fn = rtti->GetFunction("GetLocalizedTextByKey");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[Localization] Function not found: GetLocalizedTextByKey;CName");
                    return "<Function nullptr>";
                }

                CString result;
                StackArgs_t args{ { nullptr, const_cast<CName*>(&key) } };

                if (!ExecuteFunction((ScriptInstance)nullptr, fn, &result, args))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[Localization] Execution failed for key");
                    return "<Execution failed>";
                }

                return result.c_str();
            }

            inline std::string LocalizeVehicleDisplayName(const std::string& vehicleTweakID)
            {
                using namespace RED4ext;

                auto* tdb = TweakDB::Get();
                if (!tdb)
                    return "<TweakDB unavailable>";

                std::string fullKey = vehicleTweakID + ".displayName";
                TweakDBID displayNameID(fullKey.c_str());

                gamedataLocKeyWrapper locKey;
                if (!tdb->TryGetValue(displayNameID, locKey))
                    return "<Missing displayName>";

                return GetLocalizedTextByKey(CName(locKey.primaryKey));
            }

            inline std::string DumpVehicleRecordInfo(const std::string& vehicleTweakID)
            {
                auto* tdb = RED4ext::TweakDB::Get();
                if (!tdb)
                    return "<TweakDB unavailable>";

                TweakDBID recordID(vehicleTweakID.c_str());
                RED4ext::Handle<RED4ext::gamedataTweakDBRecord> record;
                if (!tdb->TryGetRecord(recordID, record) || !record)
                    return "<Record not found>";

                auto* rtti = RED4ext::CRTTISystem::Get();
                if (!rtti)
                    return "<RTTI unavailable>";

                auto* cls = rtti->GetClass("gameVehicleOffer_Record");
                if (!cls)
                    return "<VehicleOffer_Record class missing>";

                CName brandCName;
                if (auto* brandFn = cls->GetFunction("BrandName"))
                {
                    StackArgs_t args;
                    ExecuteFunction(record, brandFn, &brandCName, args);
                }

                CString description;
                if (auto* descFn = cls->GetFunction("Description"))
                {
                    StackArgs_t args;
                    ExecuteFunction(record, descFn, &description, args);
                }

                std::string brand = Vehicle::GetLocalizedTextByKey(brandCName);
                std::string desc = description.c_str();

                std::string result = "[Brand] " + brand + " | [Desc] " + desc;

                loghandler::sdk->logger->InfoF(loghandler::handle,
                    "[VehicleInfoDump] %s > Brand: %s | Desc: %s",
                    vehicleTweakID.c_str(), brand.c_str(), desc.c_str());

                return result;
            }


        }
    }
}



/*


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

*/