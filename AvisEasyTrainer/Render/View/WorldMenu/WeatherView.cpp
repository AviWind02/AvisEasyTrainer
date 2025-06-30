#include "pch.h"
#include "Features/WorldFeat/WorldFeature.h"

using namespace UI;
using namespace Feature::World::Weather;
namespace View::World::WeatherView {

    static float transition = 1.f;
    static int priority = 5;

    static bool forceReset = false;

    void WeatherView()
    {
        Buttons::Toggle("Force Weather Reset", forceReset,
            "If enabled, the next weather reset will happen immediately. If disabled, the reset will apply during the next weather update cycle.");

        Buttons::Option("Reset Weather",
            "Triggers a weather reset.",
            [] { RequestResetWeather(forceReset); });

        Buttons::Float("Transition Duration", transition, 0.f, 75.f, 0.5f,
            "Duration in seconds for weather to transition smoothly.");

        Buttons::Int("Weather Priority", priority, 0, 10, 1,
            "Higher values override quest or scripted weather. 100 is highest.");

        Buttons::Break("", "Weather States");

        for (const auto& [label, displayName] : weatherStates)
        {
            Buttons::Option(displayName.c_str(), label.ToString(), [label]() {
                RequestSetWeather(label, transition, priority);
            });
        }
    }
}
