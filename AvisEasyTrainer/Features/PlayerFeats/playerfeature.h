#pragma once
namespace feature {
	namespace playeroptions {
		namespace playerwanted {
		
			inline bool tickNeverWanted = false;
			inline bool tickClearWanted = false;
			inline bool tickAlwaysWanted = false;
			inline bool tickHoldWanted = false;

			inline int heldWantedLevel = 0;

			// disables conflicting modes if active
			void SetNeverWantedLevel(bool enabled);
			void SetHoldWantedLevel(bool enabled);

			void SetWantedLevel(int level);
			void ClearWantedLevel();
			void Tick();

		}

		inline bool tickGodmode = false;
		inline bool tickUnlimitedStamina = false;
		inline bool tickUnlimitedMemory = false;
		inline bool tickUnlimitedOxygen = false;

		inline bool tickHealItemCooldown = false;		
		inline bool tickdetectionRate = false;
		inline bool tickTest = false;

		inline bool tickSuperJump = false;
		inline float jumpHeight = 5.f;
		
		inline bool tickKiroshEyeZoom = false;
		inline float kiroshEyeZoom = 5.f;

		inline bool tickMemoryEdit = false;
		inline int memoryValue = 0;

		inline bool tickCarryCapacity = false;
		inline int carryCapacityValue = 0;

		inline bool tickPlayerMaxSpeed = false;
		inline float maxSpeedValue = 0;

		inline bool tickFreeflyMovement = false;
		inline bool tickTraceRatelow = false;
		inline bool tickGrenadeCooldown = false;
		inline bool tickProjectileCooldown = false;
		inline bool tickCloakCooldown = false;
		inline bool tickSandevistanCooldown = false;
		inline bool tickBerserkCooldown = false;
		inline bool tickKerenzikovCooldown = false;
		inline bool tickOverclockCooldown = false;
		inline bool tickQuickhackCooldown = false;
		inline bool tickQuickhackCost = false;
		inline bool tickMemoryRegeneration = false;
		inline bool tickFaceplateCooldown = false;
		inline bool tickGodHealthRegen = false;
		inline bool tickGodArmor = false;
		inline bool tickGodFallDamage = false;
		inline bool tickGodResistances = false;
		inline bool tickGodCombatRegen = false;
		inline bool tickNoClip = false;
		void Tick();
		void showWeapons();

	}
}
