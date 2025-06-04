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
			if (!gamebase::modifier::IsModifierActive("OpticalCamoRecharge")) {
				gamebase::modifier::CreateStatModifier(game::data::StatType::OpticalCamoRechargeDuration, 0.01f, game::StatModifierType::Multiplier, "OpticalCamoRecharge");
				gamebase::modifier::AddStatModifier("OpticalCamoRecharge");
			}

			if (!gamebase::modifier::IsModifierActive("OpticalCamoRegenRate")) {
				gamebase::modifier::CreateStatModifier(game::data::StatType::OpticalCamoChargesRegenRate, 100.0f, game::StatModifierType::Additive, "OpticalCamoRegenRate");
				gamebase::modifier::AddStatModifier("OpticalCamoRegenRate");
			}

			TestCamo = gamebase::statsutils::GetStatValue(game::data::StatType::OpticalCamoDuration);
		}

		void SetQuickhackCooldownReduction()
		{
			if (!gamebase::modifier::IsModifierActive("QuickhackCooldownReduction")) {
				gamebase::modifier::CreateStatModifier(game::data::StatType::QuickhacksCooldownReduction, 1.0f, game::StatModifierType::Additive, "QuickhackCooldownReduction");
				gamebase::modifier::AddStatModifier("QuickhackCooldownReduction");
			}
		}

		void SetQuickhackUploadSpeedBoost()
		{
			if (!gamebase::modifier::IsModifierActive("QuickhackUploadTimeDecrease")) {
				gamebase::modifier::CreateStatModifier(game::data::StatType::QuickHackUploadTimeDecrease, 2.0f, game::StatModifierType::Multiplier, "QuickhackUploadTimeDecrease");
				gamebase::modifier::AddStatModifier("QuickhackUploadTimeDecrease");
			}
		}

		void SetQuickhackQueueCountBoost()
		{
			if (!gamebase::modifier::IsModifierActive("QuickhackQueueCount")) {
				gamebase::modifier::CreateStatModifier(game::data::StatType::QuickHackQueueCount, 5.0f, game::StatModifierType::Additive, "QuickhackQueueCount");
				gamebase::modifier::AddStatModifier("QuickhackQueueCount");
			}
		}

		void SetQuickhackSpreadNumberBoost()
		{
			if (!gamebase::modifier::IsModifierActive("QuickhackSpreadNumber")) {
				gamebase::modifier::CreateStatModifier(game::data::StatType::QuickHackSpreadNumber, 3.0f, game::StatModifierType::Additive, "QuickhackSpreadNumber");
				gamebase::modifier::AddStatModifier("QuickhackSpreadNumber");
			}
		}

		void SetQuickhackSpreadDistanceBoost()
		{
			if (!gamebase::modifier::IsModifierActive("QuickhackSpreadDistanceIncrease")) {
				gamebase::modifier::CreateStatModifier(game::data::StatType::QuickHackSpreadDistanceIncrease, 50.0f, game::StatModifierType::Additive, "QuickhackSpreadDistanceIncrease");
				gamebase::modifier::AddStatModifier("QuickhackSpreadDistanceIncrease");
			}
		}

		void SetQuickhackBlackWallSpreadNumberBoost()
		{
			if (!gamebase::modifier::IsModifierActive("QuickhackBlackWallSpreadNumber")) {
				gamebase::modifier::CreateStatModifier(game::data::StatType::QuickHackBlackWallSpreadNumber, 5.0f, game::StatModifierType::Additive, "QuickhackBlackWallSpreadNumber");
				gamebase::modifier::AddStatModifier("QuickhackBlackWallSpreadNumber");
			}
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
