#include "pch.h"
#include "Base/gamebase.h"
#include "Base/Natives/Transform.h"

#include "teleportfeature.h"



namespace Feature {
	namespace TeleyOptions {


		using namespace RED4ext;
		using namespace game;
		using namespace data;
		using namespace GameBase;

        void RequestTeleport(const RED4ext::Vector3& pos)
        {
            pendingTeleportPosition = pos;
            pendingTeleportRotation = Natives::Transform::GetPlayerWorldEulerAngles();
            tickTeleportNow = true;
        }



        void TeleportForward(float distance)
        {
            Vector3 pos = Natives::Transform::GetPlayerWorldPosition();
            Vector3 rot = Natives::Transform::GetPlayerWorldEulerAngles(); 

            float yawRad = Natives::Transform::DegreeToRadian(rot.Z);
            float xOffset = distance * std::sin(yawRad) * -1.f;
            float yOffset = distance * std::cos(yawRad);

            pos.X += xOffset;
            pos.Y += yOffset;

            RequestTeleport(pos);
        }
        void TeleportUp(float amount)
        {
            Vector3 pos = Natives::Transform::GetPlayerWorldPosition();
            pos.Z += amount;
            RequestTeleport(pos);
        }

        void TeleportDown(float amount)
        {
            Vector3 pos = Natives::Transform::GetPlayerWorldPosition();
            pos.Z -= amount;
            RequestTeleport(pos);
        }

        void Tick()
        {
            if (tickTeleportNow)
            {
                Natives::Transform::SetPlayerWorldTransform(pendingTeleportPosition, { pendingTeleportRotation.X, pendingTeleportRotation.Y, pendingTeleportRotation.Z});
                tickTeleportNow = false;
            }
        }

	}
}