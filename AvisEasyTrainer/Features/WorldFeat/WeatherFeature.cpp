#include "pch.h"
#include "Base/Natives/World.h"
#include "WorldFeature.h"

namespace Feature {
    namespace World {
        namespace Weather {

            static RED4ext::CName weather = "NULL";
            static float transition = 1.f;
            static int priority = 5;

            static bool forceReset = false;

            void RequestSetWeather(RED4ext::CName weatherID, float transitionS, int priorityQ)
            {
                weather = weatherID;
                transition = transitionS;
                priority = priorityQ;
                tickSetWeatherNow = true;
            }

            void RequestResetWeather(bool force)
            {
                forceReset = force;
                tickResetWeatherNow = true;
            }

            void Tick()
            {
                if (tickSetWeatherNow)
                {
                    GameBase::Natives::World::SetWeather(weather, transition, priority);
                    tickSetWeatherNow = false;
                }

                if (tickResetWeatherNow)
                {
                    GameBase::Natives::World::ResetWeatherInternal(forceReset);
                    tickResetWeatherNow = false;
                }
            }

        }
    }
}
