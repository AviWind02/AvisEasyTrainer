#pragma once
#include "Base/GameBase.h"

#include <RED4ext/Scripting/Natives/Vector3.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/TeleportationFacility.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/IPlacedComponent.hpp>

namespace GameBase {
    namespace Natives {
        namespace Transform {


			inline Vector3 GetPlayerWorldEulerAngles()
			{
				Vector3 result{ 0.f, 0.f, 0.f };

				auto* tf = reinterpret_cast<RED4ext::ent::IPlacedComponent*>(GetPlayerEntity()->transformComponent);
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

				float pitchDegrees = pitch * (180.f / 3.14159265359f);
				float rollDegrees = roll * (180.f / 3.14159265359f);
				float yawDegrees = yaw * (180.f / 3.14159265359f);

				if (yawDegrees < 0.f)
					yawDegrees += 360.f;

				result.X = pitchDegrees;
				result.Y = rollDegrees;
				result.Z = yawDegrees;


				return result;
			}

			inline float DegreeToRadian(float angle)
			{
				return angle * 0.0174532925199433f;
			}

			inline Vector3 GetPlayerWorldPosition()
			{
				Vector3 result{ 0.f, 0.f, 0.f };

				auto* tf = reinterpret_cast<RED4ext::ent::IPlacedComponent*>(GetPlayerEntity()->transformComponent);
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
				auto teleportFacility = Systems::GetTeleportationSystem();
				if (!teleportFacility)
					return false;

				auto* rtti = RED4ext::CRTTISystem::Get();
				if (!rtti)
					return false;


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

				//loghandler::sdk->logger->InfoF(loghandler::handle, "[SetEntityWorldTransform] Teleported entity to X %.2f, Y %.2f, Z %.2f", position.X, position.Y, position.Z);
				return true;
			}

			inline bool SetPlayerWorldTransform(Vector3 position, RED4ext::EulerAngles rotation)
			{
				RED4ext::Handle<RED4ext::IScriptable> playerHandle;
				if (!TryGetPlayerHandle(playerHandle))
					return false;

				return SetEntityWorldTransform(playerHandle, position, rotation);
			}
        }
    }
}
