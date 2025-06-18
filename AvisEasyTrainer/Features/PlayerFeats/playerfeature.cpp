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

namespace feature {
	namespace playeroptions {

		using namespace RED4ext;
		using namespace game;
		using namespace data;
		using namespace gamebase;

		void SetStatPoolValue(StatPoolType stat, float value)
		{
			constexpr bool propagate = true;
			natives::statpoolmodifier::SetPoolValue(stat, value, propagate);
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

		void SetHealthRegenMods(bool remove = false)
		{
			static Handle<game::StatModifierData> regenRate;
			static Handle<game::StatModifierData> health;

			if (remove) {
				natives::statmodifier::RemoveStatModifier(regenRate);
				natives::statmodifier::RemoveStatModifier(health);
			}
			else {
				natives::statmodifier::AddStatModifier(StatType::HealthGeneralRegenRateMult, 9999.9f, StatModifierType::Additive, regenRate);
				natives::statmodifier::AddStatModifier(StatType::Health, 99999.9f, StatModifierType::Additive, health);
			}
		}

		void SetArmorMods(bool remove = false)
		{
			static Handle<game::StatModifierData> armor;
			if (remove)
				natives::statmodifier::RemoveStatModifier(armor);
			else
				natives::statmodifier::AddStatModifier(StatType::Armor, 99999999.9f, StatModifierType::Additive, armor);
		}

		void SetFallDamageMods(bool remove = false)
		{
			static Handle<game::StatModifierData> fallDamage;
			if (remove)
				natives::statmodifier::RemoveStatModifier(fallDamage);
			else
				natives::statmodifier::AddStatModifier(StatType::FallDamageReduction, 99.9f, StatModifierType::Additive, fallDamage);
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
				natives::statmodifier::RemoveStatModifier(explosion);
				natives::statmodifier::RemoveStatModifier(melee);
				natives::statmodifier::RemoveStatModifier(thermal);
				natives::statmodifier::RemoveStatModifier(chemical);
				natives::statmodifier::RemoveStatModifier(electric);
				natives::statmodifier::RemoveStatModifier(physical);
			}
			else {
				natives::statmodifier::AddStatModifier(StatType::ExplosionResistance, 99.9f, StatModifierType::Additive, explosion);
				natives::statmodifier::AddStatModifier(StatType::MeleeResistance, 99.9f, StatModifierType::Additive, melee);
				natives::statmodifier::AddStatModifier(StatType::ThermalResistance, 99.9f, StatModifierType::Additive, thermal);
				natives::statmodifier::AddStatModifier(StatType::ChemicalResistance, 99.9f, StatModifierType::Additive, chemical);
				natives::statmodifier::AddStatModifier(StatType::ElectricResistance, 99.9f, StatModifierType::Additive, electric);
				natives::statmodifier::AddStatModifier(StatType::PhysicalResistance, 99.9f, StatModifierType::Additive, physical);
			}
		}

		void SetCombatRegenMods(bool remove = false)
		{
			static Handle<game::StatModifierData> regenEnable;
			if (remove)
				natives::statmodifier::RemoveStatModifier(regenEnable);
			else
				natives::statmodifier::AddStatModifier(StatType::HealthInCombatRegenEnabled, 1.0f, StatModifierType::Additive, regenEnable);
		}

		void SetInfiniteOxygen(bool remove = false)
		{
			static Handle<game::StatModifierData> oxygen;
			if (remove)
				natives::statmodifier::RemoveStatModifier(oxygen);
			else
				natives::statmodifier::AddStatModifier(StatType::CanBreatheUnderwater, 1.0f, StatModifierType::Additive, oxygen);
		}

		void SetInfiniteStamina(bool remove = false)
		{
			static Handle<game::StatModifierData> stamina;
			if (remove)
				natives::statmodifier::RemoveStatModifier(stamina);
			else
				natives::statmodifier::AddStatModifier(StatType::CanIgnoreStamina, 1.0f, StatModifierType::Additive, stamina);
		}

		void SetHealItemCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> heal;
			if (remove)
				natives::statmodifier::RemoveStatModifier(heal);
			else
				natives::statmodifier::AddStatModifier(StatType::HealingItemsChargesRegenMult, 10000.0f, StatModifierType::Additive, heal);
		}

		void SetGrenadeCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> grenade;
			if (remove)
				natives::statmodifier::RemoveStatModifier(grenade);
			else
				natives::statmodifier::AddStatModifier(StatType::GrenadesChargesRegenMult, 10000.0f, StatModifierType::Additive, grenade);
		}

		void SetProjectileCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> projectile;
			if (remove)
				natives::statmodifier::RemoveStatModifier(projectile);
			else
				natives::statmodifier::AddStatModifier(StatType::ProjectileLauncherChargesRegenMult, 10000.0f, StatModifierType::Additive, projectile);
		}

		void SetCloakCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> cloak1;
			static Handle<game::StatModifierData> cloak2;
			if (remove)
			{
				natives::statmodifier::RemoveStatModifier(cloak1);
				natives::statmodifier::RemoveStatModifier(cloak2);
			}
			else
			{
				natives::statmodifier::AddStatModifier(StatType::OpticalCamoRechargeDuration, 0.01f, StatModifierType::Multiplier, cloak1);
				natives::statmodifier::AddStatModifier(StatType::OpticalCamoChargesRegenRate, 100.0f, StatModifierType::Additive, cloak2);
			}
		}

		void SetSandevistanCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> sand;
			if (remove)
				natives::statmodifier::RemoveStatModifier(sand);
			else
				natives::statmodifier::AddStatModifier(StatType::TimeDilationSandevistanRechargeDuration, 0.01f, StatModifierType::Multiplier, sand);
		}

		void SetBerserkCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> berserk;
			if (remove)
				natives::statmodifier::RemoveStatModifier(berserk);
			else
				natives::statmodifier::AddStatModifier(StatType::BerserkChargesRegenRate, 100.0f, StatModifierType::Additive, berserk);
		}

		void SetKerenzikovCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> keren;
			if (remove)
				natives::statmodifier::RemoveStatModifier(keren);
			else
				natives::statmodifier::AddStatModifier(StatType::KerenzikovCooldownDuration, 0.01f, StatModifierType::Multiplier, keren);
		}

		void SetOverclockCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> over1;
			static Handle<game::StatModifierData> over2;
			if (remove)
			{
				natives::statmodifier::RemoveStatModifier(over1);
				natives::statmodifier::RemoveStatModifier(over2);
			}
			else
			{
				natives::statmodifier::AddStatModifier(StatType::CyberdeckOverclockRegenRate, 100.0f, StatModifierType::Additive, over1);
				natives::statmodifier::AddStatModifier(StatType::CyberdeckOverclockCooldown, 0.01f, StatModifierType::Multiplier, over2);
			}
		}

		void SetQuickhackCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> hackcool;
			if (remove)
				natives::statmodifier::RemoveStatModifier(hackcool);
			else
				natives::statmodifier::AddStatModifier(StatType::QuickhacksCooldownReduction, 1.0f, StatModifierType::Additive, hackcool);
		}

		void SetQuickhackCost(bool remove = false)
		{
			static Handle<game::StatModifierData> hackcost;
			if (remove)
				natives::statmodifier::RemoveStatModifier(hackcost);
			else
				natives::statmodifier::AddStatModifier(StatType::MemoryCostReduction, 10000.0f, StatModifierType::Additive, hackcost);
		}

		void SetMemoryRegeneration(bool remove = false)
		{
			static Handle<game::StatModifierData> regen;
			if (remove)
				natives::statmodifier::RemoveStatModifier(regen);
			else
				natives::statmodifier::AddStatModifier(StatType::MemoryRegenRateMult, 100.0f, StatModifierType::Additive, regen);
		}

		void SetDetectionRatelow(bool remove = false)
		{
			static Handle<game::StatModifierData> detection1;
			static Handle<game::StatModifierData> detection2;
			static Handle<game::StatModifierData> detection3;

			if (remove)
			{
				natives::statmodifier::RemoveStatModifier(detection1);
				natives::statmodifier::RemoveStatModifier(detection2);
				natives::statmodifier::RemoveStatModifier(detection3);

			}
			else
			{
				// No idea if this is even right works sometimes tho soooooo yea!?
				natives::statmodifier::AddStatModifier(StatType::Visibility, 0.00001f, StatModifierType::Multiplier, detection1);
				natives::statmodifier::AddStatModifier(StatType::VisibilityReduction, 0.00001f, StatModifierType::Multiplier, detection2);
				natives::statmodifier::AddStatModifier(StatType::VisibilityReductionModifierHelper, 99999999.9f, StatModifierType::Additive, detection3);

			}
		}


		void SetSuperJump(bool remove = false)
		{
			static Handle<game::StatModifierData> jump;

			if (remove)
				natives::statmodifier::RemoveStatModifier(jump);
			else
				natives::statmodifier::AddStatModifier(StatType::JumpHeight, jumpHeight, StatModifierType::Multiplier, jump);
		}


		void SetKiroshEyeZoom(bool remove = false)
		{
			static Handle<game::StatModifierData> detection1;

			static bool loadValue = true;
			if (loadValue)
			{
				kiroshEyeZoom = natives::statmodifier::GetStatValue(StatType::KiroshiMaxZoomLevel);
				loadValue = false;
			}

			if (remove)
				natives::statmodifier::RemoveStatModifier(detection1);
			else
				natives::statmodifier::AddStatModifier(StatType::KiroshiMaxZoomLevel, kiroshEyeZoom, StatModifierType::Multiplier, detection1);
		}


		void setCarryCapacityHigh(bool remove = false)
		{
			static Handle<game::StatModifierData> carrycapacity;

			static bool valueInitialized = false;
			static int tempValue = 0;
			if (remove)
			{
				natives::statmodifier::RemoveStatModifier(carrycapacity);
			}
			else
			{

		/*		if (!valueInitialized)
				{
					tempValue = natives::statmodifier::GetStatValue(StatType::CarryCapacity);
					valueInitialized = true;
					carryCapacityValue = tempValue;
				}*/

				natives::statmodifier::AddStatModifier(StatType::CarryCapacity, carryCapacityValue, game::StatModifierType::Additive, carrycapacity);
				//natives::statmodifier::AddStatModifier(StatType::CarryCapacityModifierHelper, (memoryValue - tempValue), game::StatModifierType::Additive, memory);
			}
		}

		void setTraceRatelow(bool remove = false)
		{
			static Handle<game::StatModifierData> trace;
			static Handle<game::StatModifierData> trace2;

			if (remove)
			{
				natives::statmodifier::RemoveStatModifier(trace);
				natives::statmodifier::RemoveStatModifier(trace2);
			}
			else
			{
				natives::statmodifier::AddStatModifier(StatType::RevealNetrunnerWhenHacked, -9999.0f, game::StatModifierType::Additive, trace);
				natives::statmodifier::AddStatModifier(StatType::HasICELevelBooster, 0.0f, game::StatModifierType::Additive, trace2);// I think this kills anyone hacking player?
			}
		}
		void setMemory(bool remove = false)
		{
			static Handle<game::StatModifierData> memory;

			static bool valueInitialized = false;
			static int tempValue = 0;
			if (remove)
			{
				natives::statmodifier::RemoveStatModifier(memory);
			}
			else
			{
				// Capture the base stat once, then apply a zero modifier so the stat stays unchanged.
				// But show the value in the UI.  My math is broke
				/*if (!valueInitialized)
				{
					tempValue = natives::statmodifier::GetStatValue(StatType::Memory);
					valueInitialized = true;
					memoryValue = tempValue;
				}*/

				natives::statmodifier::AddStatModifier(StatType::Memory, memoryValue, game::StatModifierType::Additive, memory);
			}
		}


		void setMaxSpeed(bool remove = false)
		{
			static Handle<game::StatModifierData> maxSpeedModifier;

			if (remove)
				natives::statmodifier::RemoveStatModifier(maxSpeedModifier);
			else
				natives::statmodifier::AddStatModifier(StatType::MaxSpeed, maxSpeedValue, StatModifierType::Multiplier, maxSpeedModifier);// I think this has a max no idea what it is tbh
		}

		void Settest(bool remove = false)
		{
			static Handle<game::StatModifierData> test;

			if (remove)
			{
				natives::statmodifier::RemoveStatModifier(test);

			}
			else
			{
				natives::statmodifier::InjectStatModifier(StatType::MaxSpeed, 100, game::StatModifierType::Multiplier, test);// SpeedBoost
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

			Vector3 position = gamebase::natives::transform::GetPlayerWorldPosition();
			Vector3 rotation = gamebase::natives::transform::GetPlayerWorldEulerAngles();

			yaw = rotation.Z;

			float yawRad = gamebase::natives::transform::DegreeToRadian(yaw);

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
			gamebase::natives::transform::SetPlayerWorldTransform(position, newRot);
		}


		void Tick()
		{


			playerwanted::Tick();

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


			static bool memoryEdit = false;
			static float memoryValueS = 16;
			HandleStatModifierToggle(tickMemoryEdit, (float)memoryValue, memoryValueS, memoryEdit, setMemory);

			static bool tracing = false;
			HandleStatModifierToggle(tickTraceRatelow, tracing, setTraceRatelow);

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