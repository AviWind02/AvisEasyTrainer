#pragma once

namespace Feature {
    namespace World {
        namespace Time {



            inline float currentTimeSpeedMultiplier = 1.0f;

            inline bool tickSetTimeNow = false;
            inline bool tickSyncTime = false;
            inline bool tickApplyMultiplier = false;

            void RequestSetTime(int32_t hour, int32_t minute, int32_t second);
            void SetTimeAndRequest(int32_t hour, int32_t minute, int32_t second);
            void Tick();

        }

        namespace  Weather
        {
            inline std::vector<std::pair<RED4ext::CName, std::string>> weatherStates = {
                { "24h_weather_sunny", "Sunny" },
                { "24h_weather_rain", "Rain" },
                { "24h_weather_fog", "Fog" },
                { "24h_weather_pollution", "Pollution" },
                { "24h_weather_toxic_rain", "Toxic Rain" },
                { "24h_weather_sandstorm", "Sandstorm" },
                { "24h_weather_light_clouds", "Light Clouds" },
                { "24h_weather_cloudy", "Cloudy" },
                { "24h_weather_heavy_clouds", "Heavy Clouds" },
                { "q302_deeb_blue", "Deeb Blue" },
                { "q302_light_rain", "Rain (Light)" },
                { "q302_squat_morning", "Squat Morning" },
                { "q306_epilogue_cloudy_morning", "Cloudy (Morning)" },
                { "q306_rainy_night", "Rain (Night)" },
                { "sa_courier_clouds", "Clouds (Courier)" }
            };

            inline bool tickSetWeatherNow = false;
            inline bool tickResetWeatherNow = false;

            void RequestResetWeather(bool force = false);
            void RequestSetWeather(RED4ext::CName weatherID, float transitionS = 1.f, int priorityQ = 5);
            void Tick();
        }
    }
}
