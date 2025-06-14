#pragma once

#include <Base/gamebase.h>

#include <RED4ext/Scripting/Natives/Vector3.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/TeleportationFacility.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/IPlacedComponent.hpp>

using namespace RED4ext;

namespace gamebase {
    namespace natives {
        namespace transform {


			inline Vector3 GetPlayerWorldEulerAngles()
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

/*				loghandler::sdk->logger->InfoF(loghandler::handle,
					"[GetPlayerWorldEulerAngles] Pitch: %.2f, Roll: %.2f, Yaw: %.2f",
					result.X, result.Y, result.Z)*/;

				return result;
			}

			inline Vector3 GetPlayerWorldPosition()
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

			inline bool SetEntityWorldTransform(RED4ext::Handle<RED4ext::IScriptable> entityHandle, Vector3 position, RED4ext::EulerAngles rotation)
			{
				auto teleportFacility = gamebase::GetGameSystem<game::TeleportationFacility>("GetTeleportationFacility");
				if (!teleportFacility)
				{
					loghandler::sdk->logger->Error(loghandler::handle, "[SetEntityWorldTransform] Failed to get TeleportationFacility");
					return false;
				}

				auto* rtti = RED4ext::CRTTISystem::Get();
				if (!rtti)
				{
					loghandler::sdk->logger->Error(loghandler::handle, "[SetEntityWorldTransform] RTTI system is null");
					return false;
				}

				auto* teleportFn = rtti->GetClass("gameTeleportationFacility")->GetFunction("Teleport");
				if (!teleportFn)
				{
					loghandler::sdk->logger->Error(loghandler::handle, "[SetEntityWorldTransform] Function Teleport not found");
					return false;
				}

				RED4ext::Vector4 posVec{ position.X, position.Y, position.Z, 1.0f };
				StackArgs_t args{
					{ nullptr, &entityHandle },
					{ nullptr, &posVec },
					{ nullptr, &rotation }
				};

				if (!ExecuteFunction(teleportFacility, teleportFn, nullptr, args))
				{
					loghandler::sdk->logger->Error(loghandler::handle, "[SetEntityWorldTransform] Failed to execute Teleport");
					return false;
				}

				loghandler::sdk->logger->InfoF(loghandler::handle, "[SetEntityWorldTransform] Teleported entity to X %.2f, Y %.2f, Z %.2f", position.X, position.Y, position.Z);
				return true;
			}

			inline bool SetPlayerWorldTransform(Vector3 position, RED4ext::EulerAngles rotation)
			{
				RED4ext::Handle<RED4ext::IScriptable> playerHandle;
				if (!gamebase::TryGetPlayerHandle(playerHandle))
				{
					loghandler::sdk->logger->Error(loghandler::handle, "[SetPlayerWorldTransform] Failed to get player handle");
					return false;
				}

				return SetEntityWorldTransform(playerHandle, position, rotation);
			}
        }
    }
}
