#include "pch.h"
#include "Features/Base/gamebase.h"
#include "playerfeature.h"

namespace feature {
	namespace playeroptions {

		using namespace RED4ext;
		using namespace RED4ext::game::data;

		void SetStatPoolValue(StatPoolType stat, float value)
		{
			constexpr bool propagate = true;
			gamebase::statsutils::SetPoolValue(stat, value, propagate);
		}

		void SetHealthFull()
		{
			SetStatPoolValue(StatPoolType::Health, 100.f);
		}

		void SetStaminaFull()
		{
			SetStatPoolValue(StatPoolType::Stamina, 100.f);
		}

		void SetMemoryFull()
		{
			SetStatPoolValue(StatPoolType::Memory, 100.f);
		}

		void SetThrowRecoveryFull()
		{
			SetStatPoolValue(StatPoolType::ThrowRecovery, 100.f);
		}

		void SetOxygenFull()
		{
			SetStatPoolValue(StatPoolType::Oxygen, 100.f);
		}
		void SetOpticalCamoFull()
		{
			gamebase::modifier::AddStatModifier(game::data::StatType::OpticalCamoRechargeDuration, 0.01f, game::StatModifierType::Multiplier);
			gamebase::modifier::AddStatModifier(game::data::StatType::OpticalCamoChargesRegenRate, 100.0f, game::StatModifierType::Additive);

			TestCamo = gamebase::statsutils::GetStatValue(game::data::StatType::OpticalCamoRechargeDuration);
		}

		void SetQuickhackCooldownReduction()
		{
			gamebase::modifier::AddStatModifier(game::data::StatType::QuickhacksCooldownReduction, 1.0f, game::StatModifierType::Additive);
		}

		void SetQuickhackUploadSpeedBoost()
		{
			gamebase::modifier::AddStatModifier(game::data::StatType::QuickHackUploadTimeDecrease, 2.0f, game::StatModifierType::Multiplier);
		}

		void SetQuickhackQueueCountBoost()
		{
			gamebase::modifier::AddStatModifier(game::data::StatType::QuickHackQueueCount, 5.0f, game::StatModifierType::Additive);
		}

		void SetQuickhackSpreadNumberBoost()
		{
			gamebase::modifier::AddStatModifier(game::data::StatType::QuickHackSpreadNumber, 3.0f, game::StatModifierType::Additive);
		}

		void SetQuickhackSpreadDistanceBoost()
		{
			gamebase::modifier::AddStatModifier(game::data::StatType::QuickHackSpreadDistanceIncrease, 50.0f, game::StatModifierType::Additive);
		}

		void SetQuickhackBlackWallSpreadNumberBoost()
		{
			gamebase::modifier::AddStatModifier(game::data::StatType::QuickHackBlackWallSpreadNumber, 5.0f, game::StatModifierType::Additive);
		}


		void Tick()
		{
			if (tickGodmode)
				SetHealthFull();

			if (tickUnlimitedStamina)
				SetStaminaFull();

			if (tickUnlimitedMemory)
				SetMemoryFull();

			if (tickFastThrowRecovery)
				SetThrowRecoveryFull();

			if (tickUnlimitedOxygen)
				SetOxygenFull();

			if (tickUnlimitedOpticalCamo)
				SetOpticalCamoFull();

			if (tickQuickhackCooldownReduction)
				SetQuickhackCooldownReduction();

			if (tickQuickhackUploadSpeedBoost)
				SetQuickhackUploadSpeedBoost();

			if (tickQuickhackQueueCountBoost)
				SetQuickhackQueueCountBoost();

			if (tickQuickhackSpreadNumberBoost)
				SetQuickhackSpreadNumberBoost();

			if (tickQuickhackSpreadDistanceBoost)
				SetQuickhackSpreadDistanceBoost();

			if (tickQuickhackBlackWallSpreadNumberBoost)
				SetQuickhackBlackWallSpreadNumberBoost();
		}


	}
}
