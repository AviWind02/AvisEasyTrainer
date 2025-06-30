#include "pch.h"
#include "Features/WorldFeat/WorldFeature.h"

using namespace UI;

namespace View::World::TimeView {

    static int hourInput = 0;
    static int minuteInput = 0;
    static int secondInput = 0;

    void TimeView()
    {
        using namespace Feature::World::Time;

        Buttons::Int("Hour", hourInput, 0, 23, 1, "Set the current hour (0–23)", [] {
            RequestSetTime(hourInput, minuteInput, secondInput);
        });

        Buttons::Int("Minute", minuteInput, 0, 59, 1, "Set the current minute (0–59)", [] {
            RequestSetTime(hourInput, minuteInput, secondInput);
        });

        Buttons::Int("Second", secondInput, 0, 59, 1, "Set the current second (0–59)", [] {
            RequestSetTime(hourInput, minuteInput, secondInput);
        });
        Buttons::Toggle("Synchronize with System Time", tickSyncTime, "Updates in-game time to match real-world clock.");
  

        Buttons::Break("", "Presets");
        Buttons::Option("Morning (6:00:00)", "Sets the time to early morning (6 AM).", [] {
            SetTimeAndRequest(6, 0, 0);
            });

        Buttons::Option("Noon (12:00:00)", "Sets the time to midday (12 PM).", [] {
            SetTimeAndRequest(12, 0, 0);
            });

        Buttons::Option("Afternoon (15:00:00)", "Sets the time to afternoon (3 PM).", [] {
            SetTimeAndRequest(15, 0, 0);
            });

        Buttons::Option("Evening (18:00:00)", "Sets the time to evening (6 PM).", [] {
            SetTimeAndRequest(18, 0, 0);
            });

        Buttons::Option("Night (21:00:00)", "Sets the time to night (9 PM).", [] {
            SetTimeAndRequest(21, 0, 0);
            });
        Buttons::Break("", "Quick Hours");
        for (int h = 0; h < 24; ++h)
        {
            int hour12 = h % 12 == 0 ? 12 : h % 12;
            const char* ampm = h < 12 ? "AM" : "PM";
            char label[32];
            std::snprintf(label, sizeof(label), "%d:00 %s", hour12, ampm);

            char desc[64];
            std::snprintf(desc, sizeof(desc), "Set the time to exactly %02d:00:00.", h);

            char rightText[16];
            std::snprintf(rightText, sizeof(rightText), "%02d:00", h);

            Buttons::OptionExtended(label, "|", rightText, desc, [h] {
                SetTimeAndRequest(h, 0, 0);
                });
        }
    }

    SubMenu timeControlMenu{ "Time Control", &TimeView };

}
