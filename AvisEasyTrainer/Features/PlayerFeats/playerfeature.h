#pragma once
namespace feature {
	namespace playeroptions {


		inline bool tickGodmode = false;
		inline bool tickUnlimitedStamina = false;
		inline bool tickUnlimitedMemory = false;
		inline bool tickFastThrowRecovery = false;
		inline bool tickUnlimitedOxygen = false;
		inline bool tickUnlimitedOpticalCamo = false;

		inline bool tickQuickhackCooldownReduction = false;
		inline bool tickQuickhackUploadSpeedBoost = false;
		inline bool tickQuickhackQueueCountBoost = false;
		inline bool tickQuickhackSpreadNumberBoost = false;
		inline bool tickQuickhackSpreadDistanceBoost = false;
		inline bool tickQuickhackBlackWallSpreadNumberBoost = false;

		inline float TestCamo = -1.f;
		void SetHealthFull();
		void SetStaminaFull();
		void SetMemoryFull();
		void SetThrowRecoveryFull();
		void SetOxygenFull();
		void SetOpticalCamoFull();

		void Tick();

	}
}
