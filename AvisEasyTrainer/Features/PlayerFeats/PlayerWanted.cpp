#include "pch.h"
#include "Base/gamebase.h"
#include "playerfeature.h"

#include "Base/Natives/preventionsystem.h"

namespace feature {
	namespace playeroptions {
		namespace playerwanted {

			using namespace RED4ext;
			using namespace game;
			using namespace data;
			using namespace gamebase;


			using namespace natives::prevention;
			void SetWantedLevel(int level)
			{
				level = std::clamp(level, 1, 5);
				SetPlayerWantedLevel(level);
				heldWantedLevel = level;
			}

			void ClearWantedLevel()
			{
				DisablePoliceSystem(true);
				heldWantedLevel = 0;
				DisablePoliceSystem(false);
			}

			void SetNeverWantedLevel(bool enabled)
			{
				if (enabled && tickHoldWanted)
				{
					tickHoldWanted = false;
					render::ui::NotificationSystem::NotifyInfo("Disabled: Hold Wanted Level");
				}

				tickNeverWanted = enabled;
			}

			void SetHoldWantedLevel(bool enabled)
			{
				if (enabled && tickNeverWanted)
				{
					tickNeverWanted = false;
					render::ui::NotificationSystem::NotifyInfo("Disabled: Never Wanted");
				}

				tickHoldWanted = enabled;
			}


            void Tick()
            {
				static bool wasSystemDisabled = false;

                if (tickClearWanted)
                {
                    ClearWantedLevel();
					tickHoldWanted = false;
					tickClearWanted = false;


                }

                if (tickNeverWanted)
                {
                    DisablePoliceSystem(true); 
                    heldWantedLevel = 0;
					wasSystemDisabled = true;

                }
				else
				{
					if (wasSystemDisabled)
					{

						DisablePoliceSystem(false);
						wasSystemDisabled = false;
					}
				}


				if (heldWantedLevel >= 1)
				{
					if (tickHoldWanted)
						SetWantedLevel(heldWantedLevel); // Hold
					DisablePoliceSystem(false);
				}
	
            }
		}
	}
}