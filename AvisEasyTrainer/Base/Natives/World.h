#pragma once
#include "Base/GameBase.h"

#include <RED4ext/Scripting/Natives/Generated/game/TimeSystem.hpp>

namespace GameBase {
    namespace Natives {
        namespace World {

            inline bool SetTime(int32_t hours, int32_t minutes, int32_t seconds) {
                StackArgs_t args{
                    { nullptr, &hours },
                    { nullptr, &minutes },
                    { nullptr, &seconds },
                };

                bool result = false;
                ExecuteFunction("gameTimeSystem", "SetGameTimeByHMS", &result, args);
                return result;
            }

            inline void SetWeather(CName weatherName, float transitionS = 1.f, int c = 5)
            {


                auto weatherSystem = Systems::GetWeatherSystem();
                if (!weatherSystem)
                    return;

                auto* rtti = RED4ext::CRTTISystem::Get();
                auto* cls = rtti->GetClass("worldWeatherScriptInterface");
                if (!cls)
                    return;


                auto* fn = cls->GetFunction("SetWeather");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWeather] SetWeather function not found");
                    return;
                }

                StackArgs_t args{
                    { nullptr, &weatherName },
                    { nullptr, &transitionS },
                    { nullptr, &transitionS }
                };
                bool res = false;
                if (!ExecuteFunction(weatherSystem, fn, &res, args))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWeather] Failed to execute SetWeather");
                    return;
                }

            }


            inline bool ResetWeatherInternal(bool force = true)
            {
                auto weatherSystem = Systems::GetWeatherSystem();
                if (!weatherSystem)
                    return false;

                auto* rtti = RED4ext::CRTTISystem::Get();
                auto* cls = rtti->GetClass("worldWeatherScriptInterface");
                if (!cls)
                    return false;

                auto* fn = cls->GetFunction("ResetWeather");
                if (!fn)
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[ResetWeatherInternal] ResetWeather function not found");
                    return false;
                }

                StackArgs_t args{
                    { nullptr, &force }
                };

                bool res = false;// Idk if these are even bool return types
                if (!ExecuteFunction(weatherSystem, fn, &res, args))
                {
                    loghandler::sdk->logger->Error(loghandler::handle, "[SetWeather] Failed to execute SetWeather");
                    return false;
                }
                return res;
            }
        }
    }
}
