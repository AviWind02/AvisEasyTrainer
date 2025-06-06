#include "pch.h"
#include "Features/Base/gamebase.h"
#include "playerfeature.h"
#include <RED4ext/Scripting/Natives/Generated/ent/IPlacedComponent.hpp>

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


		void SetOxygenFull()
		{
			SetStatPoolValue(StatPoolType::Oxygen, 100.f);
		}

		void SetHealthRegenMods(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> regenRate;
			static Handle<game::StatModifierData> health;

			if (remove) {
				RemoveStatModifier(regenRate);
				RemoveStatModifier(health);
			}
			else {
				AddStatModifier(StatType::HealthGeneralRegenRateMult, 9999.9f, StatModifierType::Additive, regenRate);
				AddStatModifier(StatType::Health, 99999.9f, StatModifierType::Additive, health);
			}
		}

		void SetArmorMods(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> armor;
			if (remove)
				RemoveStatModifier(armor);
			else
				AddStatModifier(StatType::Armor, 99999999.9f, StatModifierType::Additive, armor);
		}

		void SetFallDamageMods(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> fallDamage;
			if (remove)
				RemoveStatModifier(fallDamage);
			else
				AddStatModifier(StatType::FallDamageReduction, 99.9f, StatModifierType::Additive, fallDamage);
		}

		void SetDamageResistances(bool remove = false)
		{
			using namespace gamebase::modifier;

			static Handle<game::StatModifierData> explosion;
			static Handle<game::StatModifierData> melee;
			static Handle<game::StatModifierData> thermal;
			static Handle<game::StatModifierData> chemical;
			static Handle<game::StatModifierData> electric;
			static Handle<game::StatModifierData> physical;

			if (remove) {
				RemoveStatModifier(explosion);
				RemoveStatModifier(melee);
				RemoveStatModifier(thermal);
				RemoveStatModifier(chemical);
				RemoveStatModifier(electric);
				RemoveStatModifier(physical);
			}
			else {
				AddStatModifier(StatType::ExplosionResistance, 99.9f, StatModifierType::Additive, explosion);
				AddStatModifier(StatType::MeleeResistance, 99.9f, StatModifierType::Additive, melee);
				AddStatModifier(StatType::ThermalResistance, 99.9f, StatModifierType::Additive, thermal);
				AddStatModifier(StatType::ChemicalResistance, 99.9f, StatModifierType::Additive, chemical);
				AddStatModifier(StatType::ElectricResistance, 99.9f, StatModifierType::Additive, electric);
				AddStatModifier(StatType::PhysicalResistance, 99.9f, StatModifierType::Additive, physical);
			}
		}

		void SetCombatRegenMods(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> regenEnable;
			if (remove)
				RemoveStatModifier(regenEnable);
			else
				AddStatModifier(StatType::HealthInCombatRegenEnabled, 1.0f, StatModifierType::Additive, regenEnable);
		}

		void SetInfiniteOxygen(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> oxygen;
			if (remove)
				RemoveStatModifier(oxygen);
			else
				AddStatModifier(StatType::CanBreatheUnderwater, 1.0f, StatModifierType::Additive, oxygen);
		}

		void SetInfiniteStamina(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> stamina;
			if (remove)
				RemoveStatModifier(stamina);
			else
				AddStatModifier(StatType::CanIgnoreStamina, 1.0f, StatModifierType::Additive, stamina);
		}

		void SetHealItemCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> heal;
			if (remove)
				RemoveStatModifier(heal);
			else
				AddStatModifier(StatType::HealingItemsChargesRegenMult, 10000.0f, StatModifierType::Additive, heal);
		}

		void SetGrenadeCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> grenade;
			if (remove)
				RemoveStatModifier(grenade);
			else
				AddStatModifier(StatType::GrenadesChargesRegenMult, 10000.0f, StatModifierType::Additive, grenade);
		}

		void SetProjectileCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> projectile;
			if (remove)
				RemoveStatModifier(projectile);
			else
				AddStatModifier(StatType::ProjectileLauncherChargesRegenMult, 10000.0f, StatModifierType::Additive, projectile);
		}

		void SetCloakCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> cloak1;
			static Handle<game::StatModifierData> cloak2;
			if (remove)
			{
				RemoveStatModifier(cloak1);
				RemoveStatModifier(cloak2);
			}
			else
			{
				AddStatModifier(StatType::OpticalCamoRechargeDuration, 0.01f, StatModifierType::Multiplier, cloak1);
				AddStatModifier(StatType::OpticalCamoChargesRegenRate, 100.0f, StatModifierType::Additive, cloak2);
			}
		}

		void SetSandevistanCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> sand;
			if (remove)
				RemoveStatModifier(sand);
			else
				AddStatModifier(StatType::TimeDilationSandevistanRechargeDuration, 0.01f, StatModifierType::Multiplier, sand);
		}

		void SetBerserkCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> berserk;
			if (remove)
				RemoveStatModifier(berserk);
			else
				AddStatModifier(StatType::BerserkChargesRegenRate, 100.0f, StatModifierType::Additive, berserk);
		}

		void SetKerenzikovCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> keren;
			if (remove)
				RemoveStatModifier(keren);
			else
				AddStatModifier(StatType::KerenzikovCooldownDuration, 0.01f, StatModifierType::Multiplier, keren);
		}

		void SetOverclockCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> over1;
			static Handle<game::StatModifierData> over2;
			if (remove)
			{
				RemoveStatModifier(over1);
				RemoveStatModifier(over2);
			}
			else
			{
				AddStatModifier(StatType::CyberdeckOverclockRegenRate, 100.0f, StatModifierType::Additive, over1);
				AddStatModifier(StatType::CyberdeckOverclockCooldown, 0.01f, StatModifierType::Multiplier, over2);
			}
		}

		void SetQuickhackCooldown(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> hackcool;
			if (remove)
				RemoveStatModifier(hackcool);
			else
				AddStatModifier(StatType::QuickhacksCooldownReduction, 1.0f, StatModifierType::Additive, hackcool);
		}

		void SetQuickhackCost(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> hackcost;
			if (remove)
				RemoveStatModifier(hackcost);
			else
				AddStatModifier(StatType::MemoryCostReduction, 10000.0f, StatModifierType::Additive, hackcost);
		}

		void SetMemoryRegeneration(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> regen;
			if (remove)
				RemoveStatModifier(regen);
			else
				AddStatModifier(StatType::MemoryRegenRateMult, 100.0f, StatModifierType::Additive, regen);
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

		void Tick()
		{
			if (tickGodmode)
				SetHealthFull();

			if (tickUnlimitedStamina)
				SetStaminaFull();

			if (tickUnlimitedMemory)
				SetMemoryFull();

			if (tickUnlimitedOxygen)
				SetOxygenFull();
			
			//static bool appliedOxygen = false;
			//HandleStatModifierToggle(tickUnlimitedOxygen, appliedOxygen, SetInfiniteOxygen);

			//static bool appliedStamina = false;
			//HandleStatModifierToggle(tickUnlimitedStamina, appliedStamina, SetInfiniteStamina);

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




/*
	



		Vector3 GetForwardFromQuat(const RED4ext::Quaternion& q)
		{
			return {
				2 * (q.i * q.k + q.r * q.j),
				2 * (q.j * q.k - q.r * q.i),
				1 - 2 * (q.i * q.i + q.j * q.j)
			};
		}

		Vector3 GetRightFromQuat(const RED4ext::Quaternion& q)
		{
			return {
				1 - 2 * (q.j * q.j + q.k * q.k),
				2 * (q.i * q.j + q.r * q.k),
				2 * (q.i * q.k - q.r * q.j)
			};
		}


		RED4ext::Vector3 Add(const RED4ext::Vector3& a, const RED4ext::Vector3& b)
		{
			return RED4ext::Vector3{ a.X + b.X, a.Y + b.Y, a.Z + b.Z };
		}

		RED4ext::Vector3 Subtract(const RED4ext::Vector3& a, const RED4ext::Vector3& b)
		{
			return RED4ext::Vector3{ a.X - b.X, a.Y - b.Y, a.Z - b.Z };
		}

		RED4ext::Vector3 Multiply(const RED4ext::Vector3& v, float scalar)
		{
			return RED4ext::Vector3{ v.X * scalar, v.Y * scalar, v.Z * scalar };
		}

		RED4ext::Vector3 Divide(const RED4ext::Vector3& v, float scalar)
		{
			return RED4ext::Vector3{ v.X / scalar, v.Y / scalar, v.Z / scalar };
		}

		void AddInPlace(RED4ext::Vector3& a, const RED4ext::Vector3& b)
		{
			a.X += b.X;
			a.Y += b.Y;
			a.Z += b.Z;
		}

		void SubtractInPlace(RED4ext::Vector3& a, const RED4ext::Vector3& b)
		{
			a.X -= b.X;
			a.Y -= b.Y;
			a.Z -= b.Z;
		}

		void MultiplyInPlace(RED4ext::Vector3& v, float scalar)
		{
			v.X *= scalar;
			v.Y *= scalar;
			v.Z *= scalar;
		}

		void DivideInPlace(RED4ext::Vector3& v, float scalar)
		{
			v.X /= scalar;
			v.Y /= scalar;
			v.Z /= scalar;
		}

		float noclipBaseSpeed = 8.0f;
		float noclipBoostMultiplier = 2.5f;


		bool IsKeyPressed(int key)
		{
			return (GetAsyncKeyState(key) & 0x8000) != 0;
		}

		bool KeyW()
		{
			return IsKeyPressed('W');
		}

		bool KeyA()
		{
			return IsKeyPressed('A');
		}

		bool KeyS()
		{
			return IsKeyPressed('S');
		}

		bool KeyD()
		{
			return IsKeyPressed('D');
		}

		bool SpaceHeld()
		{
			return IsKeyPressed(VK_SPACE);
		}

		bool CtrlHeld()
		{
			return IsKeyPressed(VK_LCONTROL) || IsKeyPressed(VK_RCONTROL);
		}

		bool ShiftHeld()
		{
			return IsKeyPressed(VK_LSHIFT) || IsKeyPressed(VK_RSHIFT);
		}



*/