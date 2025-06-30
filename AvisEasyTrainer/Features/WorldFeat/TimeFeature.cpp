#include "pch.h"
#include "Base/gamebase.h"
#include "WorldFeature.h"

namespace Feature {
    namespace World {
        namespace Time {

            static int32_t requestedHour = 0;
            static int32_t requestedMinute = 0;
            static int32_t requestedSecond = 0;

            void SetTimeAndRequest(int32_t hour, int32_t minute, int32_t second)
            {
                StackArgs_t args{
                    { nullptr, &hour },
                    { nullptr, &minute },
                    { nullptr, &second }
                };
                ExecuteFunction("gameTimeSystem", "SetGameTimeByHMS", nullptr, args);
            }

            void RequestSetTime(int32_t hour, int32_t minute, int32_t second)
            {
                requestedHour = hour;
                requestedMinute = minute;
                requestedSecond = second;
                tickSetTimeNow = true;
            }

            void Tick()
            {
                if (tickSyncTime)
                {
                    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::tm localTime{};
                    localtime_s(&localTime, &now);
                    RequestSetTime(localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
                }

                if (tickSetTimeNow)
                {
                    SetTimeAndRequest(requestedHour, requestedMinute, requestedSecond);
                    tickSetTimeNow = false;
                }
            }
        }
    }
}
