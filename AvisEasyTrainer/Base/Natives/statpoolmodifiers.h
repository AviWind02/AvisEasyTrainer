#pragma once

#include <Base/gamebase.h>

#include <RED4ext/Scripting/Natives/Generated/game/StatPoolsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatPoolType.hpp>

using namespace RED4ext;

namespace gamebase {
    namespace natives {
        namespace statpoolmodifier {


            inline float GetPoolValue(game::data::StatPoolType poolType) {
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

            inline bool SetPoolValue(game::data::StatPoolType poolType, float newValue, bool propagate) {
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
        }
    }
}