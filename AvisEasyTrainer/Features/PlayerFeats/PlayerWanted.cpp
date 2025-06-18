#include "pch.h"
#include "Base/gamebase.h"
#include "playerfeature.h"

#include "Base/Natives/preventionsystem.h"

// Some of the functions below were inspired by or adapted from the CET SimpleMenu Lua scripts,
// originally created by Dank Rafft and capncoolio2. 

// I'm using their script examples as a reference for which stat values to apply and how features like
// InfiniteOxygen, InfiniteStamina, HealItemCooldown, GrenadeCooldown, ProjectileCooldown, CloakCooldown,
// SandevistanCooldown, BerserkCooldown, KerenzikovCooldown, OverclockCooldown, QuickhackCooldown,
// QuickhackCost, MemoryRegeneration, and the original GodMode are handled.

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
				level = std::clamp(level, 0, 5);
				SetPlayerWantedLevel(level);
				heldWantedLevel = level;
			}

			void ClearWantedLevel()
			{
				TurnOffWantedLevel();
				heldWantedLevel = 0;
			}

			void Tick()
			{

				if (tickAlwaysWanted && heldWantedLevel != 5)
					SetWantedLevel(5);

				if (tickNeverWanted)
				{
					heldWantedLevel = 0;
					ClearWantedLevel();
				}
				if (tickHoldWanted && heldWantedLevel > 0)
					SetWantedLevel(heldWantedLevel);
			}
		}
	}
}