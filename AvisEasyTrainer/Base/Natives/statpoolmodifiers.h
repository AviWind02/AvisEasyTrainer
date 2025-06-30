#pragma once
#include "Base/GameBase.h"

#include <RED4ext/Scripting/Natives/Generated/game/StatPoolsSystem.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/StatPoolType.hpp>

namespace GameBase {
    namespace Natives {
        namespace StatPoolModifier {


            inline float GetPoolValue(game::data::StatPoolType poolType) {
                Handle<IScriptable> playerHandle;
                ent::EntityID playerID;

                if (!TryGetPlayerHandleAndID(playerHandle, playerID)) 
                    return -1.f;

                auto statPoolSystem = Systems::GetStatPoolsSystem();
                if (!statPoolSystem) 
                    return -1.f;

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
                if (!ExecuteFunction(statPoolSystem, fn, &outValue, args)) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[GetPoolValue] Execution failed");
                    return -1.f;
                }
                return outValue;
            }

            inline bool SetPoolValue(game::data::StatPoolType poolType, float newValue, bool propagate) {
                Handle<IScriptable> playerHandle;
                ent::EntityID playerID;

                if (!TryGetPlayerHandleAndID(playerHandle, playerID)) 
                    return false;

                auto statPoolSystem = Systems::GetStatPoolsSystem();
                if (!statPoolSystem) {
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

                if (!ExecuteFunction(statPoolSystem, fn, nullptr, args)) {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetPoolValue] Execution failed");
                    return false;
                }
                    return true;
            }
        }
    }
}