#include "pch.h"
#include "Features/Base/gamebase.h"
#include "playerfeature.h"
#include <RED4ext/Scripting/Natives/Generated/ent/IPlacedComponent.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/data/WeaponItem_Record.hpp>

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
		using namespace modifier;

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
			static Handle<game::StatModifierData> regenEnable;
			if (remove)
				RemoveStatModifier(regenEnable);
			else
				AddStatModifier(StatType::HealthInCombatRegenEnabled, 1.0f, StatModifierType::Additive, regenEnable);
		}

		void SetInfiniteOxygen(bool remove = false)
		{
			static Handle<game::StatModifierData> oxygen;
			if (remove)
				RemoveStatModifier(oxygen);
			else
				AddStatModifier(StatType::CanBreatheUnderwater, 1.0f, StatModifierType::Additive, oxygen);
		}

		void SetInfiniteStamina(bool remove = false)
		{
			static Handle<game::StatModifierData> stamina;
			if (remove)
				RemoveStatModifier(stamina);
			else
				AddStatModifier(StatType::CanIgnoreStamina, 1.0f, StatModifierType::Additive, stamina);
		}

		void SetHealItemCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> heal;
			if (remove)
				RemoveStatModifier(heal);
			else
				AddStatModifier(StatType::HealingItemsChargesRegenMult, 10000.0f, StatModifierType::Additive, heal);
		}

		void SetGrenadeCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> grenade;
			if (remove)
				RemoveStatModifier(grenade);
			else
				AddStatModifier(StatType::GrenadesChargesRegenMult, 10000.0f, StatModifierType::Additive, grenade);
		}

		void SetProjectileCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> projectile;
			if (remove)
				RemoveStatModifier(projectile);
			else
				AddStatModifier(StatType::ProjectileLauncherChargesRegenMult, 10000.0f, StatModifierType::Additive, projectile);
		}

		void SetCloakCooldown(bool remove = false)
		{
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
			static Handle<game::StatModifierData> sand;
			if (remove)
				RemoveStatModifier(sand);
			else
				AddStatModifier(StatType::TimeDilationSandevistanRechargeDuration, 0.01f, StatModifierType::Multiplier, sand);
		}

		void SetBerserkCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> berserk;
			if (remove)
				RemoveStatModifier(berserk);
			else
				AddStatModifier(StatType::BerserkChargesRegenRate, 100.0f, StatModifierType::Additive, berserk);
		}

		void SetKerenzikovCooldown(bool remove = false)
		{
			static Handle<game::StatModifierData> keren;
			if (remove)
				RemoveStatModifier(keren);
			else
				AddStatModifier(StatType::KerenzikovCooldownDuration, 0.01f, StatModifierType::Multiplier, keren);
		}

		void SetOverclockCooldown(bool remove = false)
		{
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
			static Handle<game::StatModifierData> hackcool;
			if (remove)
				RemoveStatModifier(hackcool);
			else
				AddStatModifier(StatType::QuickhacksCooldownReduction, 1.0f, StatModifierType::Additive, hackcool);
		}

		void SetQuickhackCost(bool remove = false)
		{
			static Handle<game::StatModifierData> hackcost;
			if (remove)
				RemoveStatModifier(hackcost);
			else
				AddStatModifier(StatType::MemoryCostReduction, 10000.0f, StatModifierType::Additive, hackcost);
		}

		void SetMemoryRegeneration(bool remove = false)
		{
			static Handle<game::StatModifierData> regen;
			if (remove)
				RemoveStatModifier(regen);
			else
				AddStatModifier(StatType::MemoryRegenRateMult, 100.0f, StatModifierType::Additive, regen);
		}

		void SetDetectionRatelow(bool remove = false)
		{
			static Handle<game::StatModifierData> detection1;
			static Handle<game::StatModifierData> detection2;
			static Handle<game::StatModifierData> detection3;

			if (remove)
			{
				RemoveStatModifier(detection1);
				RemoveStatModifier(detection2);
				RemoveStatModifier(detection3);

			}
			else
			{
				// No idea if this is even right works sometimes tho soooooo yea!?
				AddStatModifier(StatType::Visibility, 0.00001f, StatModifierType::Multiplier, detection1);
				AddStatModifier(StatType::VisibilityReduction, 0.00001f, StatModifierType::Multiplier, detection2);
				AddStatModifier(StatType::VisibilityReductionModifierHelper, 99999999.9f, StatModifierType::Additive, detection3);
				
			}
		}


		void SetSuperJump(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> jump;

			if (remove)
			{
				RemoveStatModifier(jump);

			}
			else
			{
				AddStatModifier(StatType::JumpHeight, jumpHeight, StatModifierType::Multiplier, jump);

			}
		}

		void SetKiroshEyeZoom(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> detection1;

			static bool loadValue = true;
			if (loadValue)
			{
				kiroshEyeZoom = statsutils::GetStatValue(StatType::KiroshiMaxZoomLevel);
				loadValue = false;
			}

			if (remove)
			{
				RemoveStatModifier(detection1);

			}
			else
			{
				AddStatModifier(StatType::KiroshiMaxZoomLevel, kiroshEyeZoom, StatModifierType::Multiplier, detection1);

			}
		}


		void setTraceRatelow(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> trace;
			static Handle<game::StatModifierData> trace2;

			if (remove)
			{
				RemoveStatModifier(trace);
				RemoveStatModifier(trace2);

			}
			else
			{
				AddStatModifier(StatType::RevealNetrunnerWhenHacked, -9999.0f, game::StatModifierType::Additive, trace);
				AddStatModifier(StatType::HasICELevelBooster, 0.0f, game::StatModifierType::Additive, trace2);// I think this kills anyone hacking player?

			}
		}

		void setMemory(bool remove = false)
		{
			using namespace gamebase::modifier;
			static Handle<game::StatModifierData> memory;

			static bool valueInitialized = false;

			if (remove)
			{
				RemoveStatModifier(memory);
			}
			else
			{
				if (!valueInitialized)
				{
					memoryValue = statsutils::GetStatValue(StatType::Memory);
					valueInitialized = true;
				}

				AddStatModifier(StatType::Memory, memoryValue, game::StatModifierType::Additive, memory);
			}
		}

		Vector3 GetPlayerWorldEulerAngles()
		{
			Vector3 result{ 0.f, 0.f, 0.f };

			auto playerEntity = gamebase::GetPlayerEntity();
			if (!playerEntity)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[GetPlayerWorldEulerAngles] Failed to get player entity");
				return result;
			}

			auto* tf = reinterpret_cast<RED4ext::ent::IPlacedComponent*>(playerEntity->transformComponent);
			if (!tf)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[GetPlayerWorldEulerAngles] transformComponent is null");
				return result;
			}

			const auto& q = tf->worldTransform.Orientation;

			float x = q.i;
			float y = q.j;
			float z = q.k;
			float w = q.r;

			float ysqr = y * y;

			float t0 = +2.0f * (w * x + y * z);
			float t1 = +1.0f - 2.0f * (x * x + ysqr);
			float roll = std::atan2(t0, t1);

			float t2 = +2.0f * (w * y - z * x);
			t2 = t2 > 1.0f ? 1.0f : t2;
			t2 = t2 < -1.0f ? -1.0f : t2;
			float pitch = std::asin(t2);

			float t3 = +2.0f * (w * z + x * y);
			float t4 = +1.0f - 2.0f * (ysqr + z * z);
			float yaw = std::atan2(t3, t4);

			// Convert all to degrees
			float pitchDegrees = pitch * (180.f / 3.14159265359f);
			float rollDegrees = roll * (180.f / 3.14159265359f);
			float yawDegrees = yaw * (180.f / 3.14159265359f);

			if (yawDegrees < 0.f)
				yawDegrees += 360.f;

			result.X = pitchDegrees;
			result.Y = rollDegrees;
			result.Z = yawDegrees;

			loghandler::sdk->logger->InfoF(loghandler::handle,
				"[GetPlayerWorldEulerAngles] Pitch: %.2f, Roll: %.2f, Yaw: %.2f",
				result.X, result.Y, result.Z);

			return result;
		}



		Vector3 GetPlayerWorldPosition()
		{
			Vector3 result{ 0.f, 0.f, 0.f };

			auto playerEntity = gamebase::GetPlayerEntity();
			if (!playerEntity)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[GetPlayerWorldPosition] Failed to get player entity");
				return result;
			}

			auto* tf = reinterpret_cast<RED4ext::ent::IPlacedComponent*>(playerEntity->transformComponent);
			if (!tf)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[GetPlayerWorldPosition] transformComponent is null");
				return result;
			}

			const auto& pos = tf->worldTransform.Position;

			constexpr float kConversion = 1.f / (2 << 16);
			result.X = pos.x.Bits * kConversion;
			result.Y = pos.y.Bits * kConversion;
			result.Z = pos.z.Bits * kConversion;

			return result;
		}

		bool SetPlayerWorldPosition(float x, float y, float z)
		{
		/*	auto playerEntity = gamebase::GetPlayerEntity();
			if (!playerEntity)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldPosition] Failed to get player entity");
				return false;
			}*/
			
			RED4ext::Handle<RED4ext::IScriptable> playerHandle;
			if (!gamebase::TryGetPlayerHandle(playerHandle))
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[TeleportPlayer] Failed to get Player handle");
				return false;
			}

			auto* rtti = RED4ext::CRTTISystem::Get();
			if (!rtti)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldPosition] RTTI system is null");
				return false;
			}


			auto* getWorldTransformFn = rtti->GetClass("entEntity")->GetFunction("GetWorldTransform");
			if (!getWorldTransformFn)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldPosition] Function GetWorldTransform not found");
				return false;
			}

			RED4ext::WorldTransform worldTransform{};
			StackArgs_t getTransformArgs;
			if (!ExecuteFunction(playerHandle, getWorldTransformFn, &worldTransform, getTransformArgs))
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldPosition] Failed to execute GetWorldTransform");
				return false;
			}

			auto* setPositionFn = rtti->GetClass("WorldTransform")->GetFunction("SetPosition");
			if (!setPositionFn)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldPosition] Function SetPosition not found");
				return false;
			}

			RED4ext::Vector4 posVec{ x, y, z, 1.0f };
			StackArgs_t setPositionArgs{
				{ nullptr, &worldTransform },
				{ nullptr, &posVec }
			};

			if (!ExecuteFunction((ScriptInstance)nullptr, setPositionFn, nullptr, setPositionArgs))
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldPosition] Failed to execute SetPosition");
				return false;
			}

			// Step 3c: SetWorldTransform
			auto* setWorldTransformFn = rtti->GetClass("entEntity")->GetFunction("SetWorldTransform");
			if (!setWorldTransformFn)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldPosition] Function SetWorldTransform not found");
				return false;
			}

			StackArgs_t setTransformArgs{ { nullptr, &worldTransform } };
			if (!ExecuteFunction(playerHandle, setWorldTransformFn, nullptr, setTransformArgs))
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldPosition] Failed to execute SetWorldTransform");
				return false;
			}

			loghandler::sdk->logger->InfoF(loghandler::handle, "[SetPlayerWorldPosition] Moved player to X %.2f, Y %.2f, Z %.2f", x, y, z);
			return true;

		}

		bool TeleportPlayer(Vector3 targetPos, RED4ext::EulerAngles rotation)
		{
			auto teleportFacility = gamebase::GetGameSystem<game::TeleportationFacility>("GetTeleportationFacility");
			if (!teleportFacility)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[TeleportPlayer] Failed to get TeleportationFacility");
				return false;
			}

			RED4ext::Handle<RED4ext::IScriptable> playerHandle;
			if (!gamebase::TryGetPlayerHandle(playerHandle))
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[TeleportPlayer] Failed to get Player handle");
				return false;
			}

			auto* rtti = RED4ext::CRTTISystem::Get();
			if (!rtti)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[TeleportPlayer] RTTI system is null");
				return false;
			}

			auto* teleportFn = rtti->GetClass("gameTeleportationFacility")->GetFunction("Teleport");
			if (!teleportFn)
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[TeleportPlayer] Function Teleport not found");
				return false;
			}

			RED4ext::Vector4 posVec{ targetPos.X, targetPos.Y, targetPos.Z, 1.0f };
			StackArgs_t args{
				{ nullptr, &playerHandle },
				{ nullptr, &posVec },
				{ nullptr, &rotation }
			};


			if (!ExecuteFunction(teleportFacility, teleportFn, nullptr, args))
			{
				loghandler::sdk->logger->Error(loghandler::handle, "[TeleportPlayer] Failed to execute Teleport");
				return false;
			}


			loghandler::sdk->logger->InfoF(loghandler::handle, "[TeleportPlayer] Teleported player to X %.2f, Y %.2f, Z %.2f", targetPos.X, targetPos.Y, targetPos.Z);
			return true;
		}



		void Settest(bool remove = false)
		{
			static Handle<game::StatModifierData> test;

			if (remove)
			{
				RemoveStatModifier(test);

			}
			else
			{
				AddStatModifier(StatType::Memory, 64, game::StatModifierType::Additive, test);
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
		void HandleFreeflyMovement()
		{
			static bool once = false;
			static Vector3 playerPos;

			static float freeflyYaw = 0.f;

			if (!once)
			{
				playerPos = GetPlayerWorldPosition();
				loghandler::sdk->logger->InfoF(loghandler::handle, "Player Pos: X %.2f, Y %.2f, Z %.2f", playerPos.X, playerPos.Y, playerPos.Z);
				once = true;
			}

			float moveStep = 0.5f;
			float rotationStep = 2.0f; 

			if (KeyA())
				freeflyYaw += rotationStep;
			if (KeyD())
				freeflyYaw -= rotationStep;

			if (freeflyYaw < 0.f)
				freeflyYaw += 360.f;
			if (freeflyYaw >= 360.f)
				freeflyYaw -= 360.f;

			float yawRad = freeflyYaw * (3.14159265f / 180.f);

			float cosYaw = cosf(yawRad);
			float sinYaw = sinf(yawRad);

			Vector3 forward = { sinYaw, cosYaw, 0.f };  
			Vector3 right = { cosYaw, -sinYaw, 0.f }; 

			if (ShiftHeld())
				moveStep += 3.f;

			if (KeyW())
			{
				playerPos.X += forward.X * moveStep;
				playerPos.Y += forward.Y * moveStep;
			}
			if (KeyS())
			{
				playerPos.X -= forward.X * moveStep;
				playerPos.Y -= forward.Y * moveStep;
			}

			float moveZStep = 0.5f;
			if (ShiftHeld())
				moveZStep += 3.f;

			if (SpaceHeld()) playerPos.Z += moveZStep;
			if (CtrlHeld()) playerPos.Z -= moveZStep;

			RED4ext::EulerAngles rotation{ 0.f, 0.f, freeflyYaw };
			TeleportPlayer(playerPos, rotation);
		}


		void Tick()
		{
			if (tickGodmode)
			{
				SetHealthFull();

				HandleFreeflyMovement();
			}


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

			//static bool kiroshEye = false;
			//static float kiroshEyeZoomS = 0.f;
			//HandleStatModifierToggle(tickKiroshEyeZoom, kiroshEyeZoom, kiroshEyeZoomS, kiroshEye, SetKiroshEyeZoom);

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