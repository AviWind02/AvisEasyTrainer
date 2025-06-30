#include "pch.h"
#include "Base/gamebase.h"
#include "playerfeature.h"

#include "Base/Natives/statmodifiers.h"
#include "Base/Natives/statpoolmodifiers.h"
#include <Base/Natives/transform.h>

// Some of the functions below were inspired by or adapted from the CET SimpleMenu Lua scripts,
// originally created by Dank Rafft and capncoolio2. 

// I'm using their script examples as a reference for which stat values to apply and how features like
// InfiniteOxygen, InfiniteStamina, HealItemCooldown, GrenadeCooldown, ProjectileCooldown, CloakCooldown,
// SandevistanCooldown, BerserkCooldown, KerenzikovCooldown, OverclockCooldown, QuickhackCooldown,
// QuickhackCost, MemoryRegeneration, and the original GodMode are handled.

namespace Feature {
	namespace PlayerOptions {

		using namespace RED4ext;
		using namespace game;
		using namespace data;
		using namespace GameBase;

		void SetStatPoolValue(StatPoolType stat, float value)
		{
			constexpr bool propagate = true;
			Natives::StatPoolModifier::SetPoolValue(stat, value, propagate);
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


		void SetOxygenFull()
		{
			SetStatPoolValue(StatPoolType::Oxygen, 100.f);
		}

		void SetQuicksilver(bool remove = false)
		{
			static Handle<game::StatModifierData>duration;
			static Handle<game::StatModifierData>timeScale;
			//static Handle<game::StatModifierData>playerSpeed;

			if (remove)
			{
				Natives::StatModifier::RemoveStatModifier(duration);
				Natives::StatModifier::RemoveStatModifier(timeScale);
				//Natives::StatModifier::RemoveStatModifier(playerSpeed);

			}
			else
			{
				Natives::StatModifier::AddStatModifier(StatType::TimeDilationSandevistanDuration, 100.f, game::StatModifierType::Multiplier, duration);
				Natives::StatModifier::AddStatModifier(StatType::TimeDilationSandevistanTimeScale, 0.005, game::StatModifierType::Multiplier, timeScale);
				//Natives::StatModifier::AddStatModifier(StatType::MaxSpeed, 15.f, game::StatModifierType::Multiplier, playerSpeed);  Better if the player just toggles its tbh
			}
		}

		void SetHealthRegenMods(bool remove = false)
		{
			static Handle<game::StatModifierData> regenRate;
			static Handle<game::StatModifierData> health;

			if (remove) {
				Natives::StatModifier::RemoveStatModifier(regenRate);
				Natives::StatModifier::RemoveStatModifier(health);
			}
			else {
				Natives::StatModifier::AddStatModifier(StatType::HealthGeneralRegenRateMult, 9999.9f, StatModifierType::Additive, regenRate);
				Natives::StatModifier::AddStatModifier(StatType::Health, 99999.9f, StatModifierType::Additive, health);
			}
		}

		void SetArmorMods(bool remove = false)
		{
			static Handle<game::StatModifierData> armor;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(armor);
			else
				Natives::StatModifier::AddStatModifier(StatType::Armor, 99999999.9f, StatModifierType::Additive, armor);
		}

		void SetFallDamageMods(bool remove = false)
		{
			static Handle<game::StatModifierData> fallDamage;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(fallDamage);
			else
				Natives::StatModifier::AddStatModifier(StatType::FallDamageReduction, 99.9f, StatModifierType::Additive, fallDamage);
		}

		void SetDamageResistances(bool remove = false)
		{
			static Handle<game::StatModifierData> explosion;
			static Handle<game::StatModifierData> melee;
			static Handle<game::StatModifierData> thermal;
			static Handle<game::StatModifierData> chemical;
			static Handle<game::StatModifierData> electric;
			static Handle<game::StatModifierData> physical;

			if (remove) {
				Natives::StatModifier::RemoveStatModifier(explosion);
				Natives::StatModifier::RemoveStatModifier(melee);
				Natives::StatModifier::RemoveStatModifier(thermal);
				Natives::StatModifier::RemoveStatModifier(chemical);
				Natives::StatModifier::RemoveStatModifier(electric);
				Natives::StatModifier::RemoveStatModifier(physical);
			}
			else {
				Natives::StatModifier::AddStatModifier(StatType::ExplosionResistance, 99.9f, StatModifierType::Additive, explosion);
				Natives::StatModifier::AddStatModifier(StatType::MeleeResistance, 99.9f, StatModifierType::Additive, melee);
				Natives::StatModifier::AddStatModifier(StatType::ThermalResistance, 99.9f, StatModifierType::Additive, thermal);
				Natives::StatModifier::AddStatModifier(StatType::ChemicalResistance, 99.9f, StatModifierType::Additive, chemical);
				Natives::StatModifier::AddStatModifier(StatType::ElectricResistance, 99.9f, StatModifierType::Additive, electric);
				Natives::StatModifier::AddStatModifier(StatType::PhysicalResistance, 99.9f, StatModifierType::Additive, physical);
			}
		}

		void SetCombatRegenMods(bool remove = false)
		{
			static Handle<game::StatModifierData> regenEnable;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(regenEnable);
			else
				Natives::StatModifier::AddStatModifier(StatType::HealthInCombatRegenEnabled, 1.0f, StatModifierType::Additive, regenEnable);
		}

		void SetInfiniteOxygen(bool remove = false)
		{
			static Handle<game::StatModifierData> oxygen;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(oxygen);
			else
				Natives::StatModifier::AddStatModifier(StatType::CanBreatheUnderwater, 1.0f, StatModifierType::Additive, oxygen);
		}

		void SetInfiniteStamina(bool remove = false)
		{
			static Handle<game::StatModifierData> stamina;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(stamina);
			else
				Natives::StatModifier::AddStatModifier(StatType::CanIgnoreStamina, 1.0f, StatModifierType::Additive, stamina);
		}

		void SetHealItemCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> heal;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(heal);
			else
				Natives::StatModifier::AddStatModifier(StatType::HealingItemsChargesRegenMult, 10000.0f, StatModifierType::Additive, heal);
		}

		void SetGrenadeCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> grenade;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(grenade);
			else
				Natives::StatModifier::AddStatModifier(StatType::GrenadesChargesRegenMult, 10000.0f, StatModifierType::Additive, grenade);
		}

		void SetProjectileCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> projectile;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(projectile);
			else
				Natives::StatModifier::AddStatModifier(StatType::ProjectileLauncherChargesRegenMult, 10000.0f, StatModifierType::Additive, projectile);
		}

		void SetCloakCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> cloak1;
			static Handle<game::StatModifierData> cloak2;
			if (remove)
			{
				Natives::StatModifier::RemoveStatModifier(cloak1);
				Natives::StatModifier::RemoveStatModifier(cloak2);
			}
			else
			{
				Natives::StatModifier::AddStatModifier(StatType::OpticalCamoRechargeDuration, 0.01f, StatModifierType::Multiplier, cloak1);
				Natives::StatModifier::AddStatModifier(StatType::OpticalCamoChargesRegenRate, 100.0f, StatModifierType::Additive, cloak2);
			}
		}

		void SetSandevistanCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> sand;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(sand);
			else
				Natives::StatModifier::AddStatModifier(StatType::TimeDilationSandevistanRechargeDuration, 0.01f, StatModifierType::Multiplier, sand);
		}

		void SetBerserkCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> berserk;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(berserk);
			else
				Natives::StatModifier::AddStatModifier(StatType::BerserkChargesRegenRate, 100.0f, StatModifierType::Additive, berserk);
		}

		void SetKerenzikovCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> keren;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(keren);
			else
				Natives::StatModifier::AddStatModifier(StatType::KerenzikovCooldownDuration, 0.01f, StatModifierType::Multiplier, keren);
		}

		void SetOverclockCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> over1;
			static Handle<game::StatModifierData> over2;
			if (remove)
			{
				Natives::StatModifier::RemoveStatModifier(over1);
				Natives::StatModifier::RemoveStatModifier(over2);
			}
			else
			{
				Natives::StatModifier::AddStatModifier(StatType::CyberdeckOverclockRegenRate, 100.0f, StatModifierType::Additive, over1);
				Natives::StatModifier::AddStatModifier(StatType::CyberdeckOverclockCooldown, 0.01f, StatModifierType::Multiplier, over2);
			}
		}

		void SetQuickhackCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> hackcool;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(hackcool);
			else
				Natives::StatModifier::AddStatModifier(StatType::QuickhacksCooldownReduction, 1.0f, StatModifierType::Additive, hackcool);
		}

		void SetQuickhackCost(bool remove = false)
		{
			static Handle<game::StatModifierData> hackcost;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(hackcost);
			else
				Natives::StatModifier::AddStatModifier(StatType::MemoryCostReduction, 10000.0f, StatModifierType::Additive, hackcost);
		}

		void SetMemoryRegeneration(bool remove = false)
		{
			static Handle<game::StatModifierData> regen;
			if (remove)
				Natives::StatModifier::RemoveStatModifier(regen);
			else
				Natives::StatModifier::AddStatModifier(StatType::MemoryRegenRateMult, 100.0f, StatModifierType::Additive, regen);
		}

		void SetDetectionRatelow(bool remove = false)
		{
			static Handle<game::StatModifierData> detection1;
			static Handle<game::StatModifierData> detection2;
			static Handle<game::StatModifierData> detection3;

			if (remove)
			{
				Natives::StatModifier::RemoveStatModifier(detection1);
				Natives::StatModifier::RemoveStatModifier(detection2);
				Natives::StatModifier::RemoveStatModifier(detection3);

			}
			else
			{
				// No idea if this is even right works sometimes tho soooooo yea!?
				Natives::StatModifier::AddStatModifier(StatType::Visibility, 0.00001f, StatModifierType::Multiplier, detection1);
				Natives::StatModifier::AddStatModifier(StatType::VisibilityReduction, 0.00001f, StatModifierType::Multiplier, detection2);
				Natives::StatModifier::AddStatModifier(StatType::VisibilityReductionModifierHelper, 99999999.9f, StatModifierType::Additive, detection3);

			}
		}


		void SetSuperJump(bool remove = false)
		{
			static Handle<game::StatModifierData> jump;

			if (remove)
				Natives::StatModifier::RemoveStatModifier(jump);
			else
				Natives::StatModifier::AddStatModifier(StatType::JumpHeight, jumpHeight, StatModifierType::Multiplier, jump);
		}


		void SetKiroshEyeZoom(bool remove = false)
		{
			static Handle<game::StatModifierData> detection1;

			static bool loadValue = true;
			if (loadValue)
			{
				kiroshEyeZoom = Natives::StatModifier::GetStatValue(StatType::KiroshiMaxZoomLevel);
				loadValue = false;
			}

			if (remove)
				Natives::StatModifier::RemoveStatModifier(detection1);
			else
				Natives::StatModifier::AddStatModifier(StatType::KiroshiMaxZoomLevel, kiroshEyeZoom, StatModifierType::Multiplier, detection1);
		}


		void setCarryCapacityHigh(bool remove = false)
		{
			static Handle<game::StatModifierData> carrycapacity;

			static bool valueInitialized = false;
			static int tempValue = 0;
			if (remove)
			{
				Natives::StatModifier::RemoveStatModifier(carrycapacity);
			}
			else
			{

		/*		if (!valueInitialized)
				{
					tempValue = Natives::StatModifier::GetStatValue(StatType::CarryCapacity);
					valueInitialized = true;
					carryCapacityValue = tempValue;
				}*/

				Natives::StatModifier::AddStatModifier(StatType::CarryCapacity, carryCapacityValue, game::StatModifierType::Additive, carrycapacity);
				//Natives::StatModifier::AddStatModifier(StatType::CarryCapacityModifierHelper, (memoryValue - tempValue), game::StatModifierType::Additive, memory);
			}
		}

		void setTraceRatelow(bool remove = false)
		{
			static Handle<game::StatModifierData> trace;
			static Handle<game::StatModifierData> trace2;

			if (remove)
			{
				Natives::StatModifier::RemoveStatModifier(trace);
				Natives::StatModifier::RemoveStatModifier(trace2);
			}
			else
			{
				Natives::StatModifier::AddStatModifier(StatType::RevealNetrunnerWhenHacked, -9999.0f, game::StatModifierType::Additive, trace);
				Natives::StatModifier::AddStatModifier(StatType::HasICELevelBooster, 0.0f, game::StatModifierType::Additive, trace2);// I think this kills anyone hacking player?
			}
		}
		void setMemory(bool remove = false)
		{
			static Handle<game::StatModifierData> memory;

			static bool valueInitialized = false;
			static int tempValue = 0;
			if (remove)
			{
				Natives::StatModifier::RemoveStatModifier(memory);
			}
			else
			{
				// Capture the base stat once, then apply a zero modifier so the stat stays unchanged.
				// But show the value in the UI.  My math is broke
				/*if (!valueInitialized)
				{
					tempValue = Natives::StatModifier::GetStatValue(StatType::Memory);
					valueInitialized = true;
					memoryValue = tempValue;
				}*/

				Natives::StatModifier::AddStatModifier(StatType::Memory, memoryValue, game::StatModifierType::Additive, memory);
			}
		}


		void setMaxSpeed(bool remove = false)
		{
			static Handle<game::StatModifierData> maxSpeedModifier;

			if (remove)
				Natives::StatModifier::RemoveStatModifier(maxSpeedModifier);
			else
				Natives::StatModifier::AddStatModifier(StatType::MaxSpeed, maxSpeedValue, StatModifierType::Multiplier, maxSpeedModifier);// I think this has a max no idea what it is tbh
		}

		void SetSandevistanDuration(bool remove = false)
		{
			static Handle<game::StatModifierData> sandevistanDuration;

			if (remove)
				Natives::StatModifier::RemoveStatModifier(sandevistanDuration);
			else
				Natives::StatModifier::AddStatModifier(StatType::TimeDilationSandevistanDuration, sandevistanDurationValue, StatModifierType::Multiplier, sandevistanDuration);
		}

		void SetSandevistanTimeScale(bool remove = false)
		{
			static Handle<game::StatModifierData> sandevistanTimeScale;

			if (remove)
				Natives::StatModifier::RemoveStatModifier(sandevistanTimeScale);
			else
				Natives::StatModifier::AddStatModifier(StatType::TimeDilationSandevistanTimeScale, sandevistanTimeScaleValue, StatModifierType::Multiplier, sandevistanTimeScale);
		}

		void Settest(bool remove = false)
		{
			static Handle<game::StatModifierData> test;

			if (remove)
			{
				Natives::StatModifier::RemoveStatModifier(test);
			}
			else
			{
				Natives::StatModifier::AddStatModifier(StatType::MagazineAutoRefill, 15.f, game::StatModifierType::Additive, test);
			}

		}

		using FeatureFn = void(*)(bool);
		void HandleStatModifierToggle(bool& toggle, bool& applied, FeatureFn applyFunc) {
			if (toggle) {
				if (!applied) {
					applyFunc(false);
					applied = true;
				}
			}
			else if (applied) {
				applyFunc(true);
				applied = false;
			}
		}

		// Adds a new mod each time the values is update and removes the previous one
		void HandleStatModifierToggle(bool toggle, float currentValue, float& lastAppliedValue, bool& applied, FeatureFn applyFunc)
		{
			if (toggle)
			{
				if (!applied)
				{
					applyFunc(false);
					applied = true;
					lastAppliedValue = currentValue;
				}
				else if (currentValue != lastAppliedValue)
				{
					applyFunc(true);
					applyFunc(false);
					lastAppliedValue = currentValue;
				}
			}
			else if (applied)
			{
				applyFunc(true); // remove
				applied = false;
			}
		}

		void TickNoclip()
		{
			static float yaw = 0.f;
			static float moveSpeed = 2.5f;
			static float rotSpeed = 1.0f;

			bool forward = ImGui::IsKeyDown(ImGuiKey_W);
			bool backward = ImGui::IsKeyDown(ImGuiKey_S);
			bool turnLeft = ImGui::IsKeyDown(ImGuiKey_A);
			bool turnRight = ImGui::IsKeyDown(ImGuiKey_D);
			bool goUp = ImGui::IsKeyDown(ImGuiKey_Space);
			bool goDown = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
			bool speedBoost = ImGui::IsKeyDown(ImGuiKey_LeftShift);

			float frameSpeed = moveSpeed * (speedBoost ? 2.5f : .7f);
			float frameRotation = rotSpeed;

			Vector3 position = Natives::Transform::GetPlayerWorldPosition();
			Vector3 rotation = Natives::Transform::GetPlayerWorldEulerAngles();

			yaw = rotation.Z;

			float yawRad = Natives::Transform::DegreeToRadian(yaw);

			if (forward)
			{
				position.X += std::sin(yawRad) * -frameSpeed;
				position.Y += std::cos(yawRad) * frameSpeed;
			}
			if (backward)
			{
				position.X -= std::sin(yawRad) * -frameSpeed;
				position.Y -= std::cos(yawRad) * frameSpeed;
			}
			if (goUp)
				position.Z += frameSpeed;
			if (goDown)
				position.Z -= frameSpeed;

			if (turnLeft)
				yaw += frameRotation;
			if (turnRight)
				yaw -= frameRotation;

			if (yaw < 0.f) yaw += 360.f;
			if (yaw > 360.f) yaw -= 360.f;

			RED4ext::EulerAngles newRot{ rotation.X, rotation.Y, yaw };
			Natives::Transform::SetPlayerWorldTransform(position, newRot);
		}


		void Tick()
		{
			PlayerWanted::Tick();

			if (tickGodmode)
				SetHealthFull();

			if (tickUnlimitedStamina)
				SetStaminaFull();

			if (tickUnlimitedMemory)
				SetMemoryFull();

			if (tickUnlimitedOxygen)
				SetOxygenFull();

			if (tickNoClip)
				TickNoclip();

			//static bool appliedOxygen = false;
			//HandleStatModifierToggle(tickUnlimitedOxygen, appliedOxygen, SetInfiniteOxygen);

			//static bool appliedStamina = false;
			//HandleStatModifierToggle(tickUnlimitedStamina, appliedStamina, SetInfiniteStamina);

			//static bool kiroshEye = false;
			//static float kiroshEyeZoomS = 0.f;
			//HandleStatModifierToggle(tickKiroshEyeZoom, kiroshEyeZoom, kiroshEyeZoomS, kiroshEye, SetKiroshEyeZoom);

			static bool maxSpeedEdit = false;
			static float maxSpeedValueSlider = 7.0f;
			HandleStatModifierToggle(tickPlayerMaxSpeed, maxSpeedValue, maxSpeedValueSlider, maxSpeedEdit, setMaxSpeed);

			static bool carryCapacityEdit = false;
			static float carryCapacityValueS = 300;
			HandleStatModifierToggle(tickCarryCapacity, (float)carryCapacityValue, carryCapacityValueS, carryCapacityEdit, setCarryCapacityHigh);

			static bool sandevistanDurationEdit = false;
			static float sandevistanDurationValueSlider = 1.f;
			HandleStatModifierToggle(tickSandevistanDuration, sandevistanDurationValue, sandevistanDurationValueSlider, sandevistanDurationEdit, SetSandevistanDuration);

			static bool sandevistanTimeScaleEdit = false;
			static float sandevistanTimeScaleValueSlider = 1.f;
			HandleStatModifierToggle(tickSandevistanTimeScale, sandevistanTimeScaleValue, sandevistanTimeScaleValueSlider, sandevistanTimeScaleEdit, SetSandevistanTimeScale);


			static bool memoryEdit = false;
			static float memoryValueS = 16;
			HandleStatModifierToggle(tickMemoryEdit, (float)memoryValue, memoryValueS, memoryEdit, setMemory);

			static bool tracing = false;
			HandleStatModifierToggle(tickTraceRatelow, tracing, setTraceRatelow);

			static bool quicksilver = false;
			HandleStatModifierToggle(tickQuicksilver, quicksilver, SetQuicksilver);


			static bool superjump = false;
			static float superjumpH = 0.f;
			HandleStatModifierToggle(tickSuperJump, jumpHeight, superjumpH, superjump, SetSuperJump);

			static bool test = false;
			HandleStatModifierToggle(tickTest, test, Settest);

			static bool detectionRate = false;
			HandleStatModifierToggle(tickdetectionRate, detectionRate, SetDetectionRatelow);

			static bool appliedHeal = false;
			HandleStatModifierToggle(tickHealItemCooldown, appliedHeal, SetHealItemCooldown);

			static bool appliedGrenade = false;
			HandleStatModifierToggle(tickGrenadeCooldown, appliedGrenade, SetGrenadeCooldown);

			static bool appliedProjectile = false;
			HandleStatModifierToggle(tickProjectileCooldown, appliedProjectile, SetProjectileCooldown);

			static bool appliedCloak = false;
			HandleStatModifierToggle(tickCloakCooldown, appliedCloak, SetCloakCooldown);

			static bool appliedSandevistan = false;
			HandleStatModifierToggle(tickSandevistanCooldown, appliedSandevistan, SetSandevistanCooldown);

			static bool appliedBerserk = false;
			HandleStatModifierToggle(tickBerserkCooldown, appliedBerserk, SetBerserkCooldown);

			static bool appliedKerenzikov = false;
			HandleStatModifierToggle(tickKerenzikovCooldown, appliedKerenzikov, SetKerenzikovCooldown);

			static bool appliedOverclock = false;
			HandleStatModifierToggle(tickOverclockCooldown, appliedOverclock, SetOverclockCooldown);

			static bool appliedQuickhackCooldown = false;
			HandleStatModifierToggle(tickQuickhackCooldown, appliedQuickhackCooldown, SetQuickhackCooldown);

			static bool appliedQuickhackCost = false;
			HandleStatModifierToggle(tickQuickhackCost, appliedQuickhackCost, SetQuickhackCost);

			static bool appliedMemoryRegen = false;
			HandleStatModifierToggle(tickMemoryRegeneration, appliedMemoryRegen, SetMemoryRegeneration);

			static bool appliedHealthRegen = false;
			HandleStatModifierToggle(tickGodHealthRegen, appliedHealthRegen, SetHealthRegenMods);

			static bool appliedArmor = false;
			HandleStatModifierToggle(tickGodArmor, appliedArmor, SetArmorMods);

			static bool appliedFall = false;
			HandleStatModifierToggle(tickGodFallDamage, appliedFall, SetFallDamageMods);

			static bool appliedResistances = false;
			HandleStatModifierToggle(tickGodResistances, appliedResistances, SetDamageResistances);

			static bool appliedCombatRegen = false;
			HandleStatModifierToggle(tickGodCombatRegen, appliedCombatRegen, SetCombatRegenMods);
		}
	}
}