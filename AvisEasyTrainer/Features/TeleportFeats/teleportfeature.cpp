#include "pch.h"
#include "Base/gamebase.h"
#include "Base/Natives/transform.h"

#include "teleportfeature.h"



namespace feature {
	namespace teleoptions {


		using namespace RED4ext;
		using namespace game;
		using namespace data;
		using namespace gamebase;

        void RequestTeleport(const RED4ext::Vector3& pos)
        {
            pendingTeleportPosition = pos;
            pendingTeleportRotation = gamebase::natives::transform::GetPlayerWorldEulerAngles();
            tickTeleportNow = true;
        }



        void TeleportForward(float distance)
        {
            Vector3 pos = gamebase::natives::transform::GetPlayerWorldPosition();
            Vector3 rot = gamebase::natives::transform::GetPlayerWorldEulerAngles(); 

            float yawRad = gamebase::natives::transform::DegreeToRadian(rot.Z);
            float xOffset = distance * std::sin(yawRad) * -1.f;
            float yOffset = distance * std::cos(yawRad);

            pos.X += xOffset;
            pos.Y += yOffset;

            RequestTeleport(pos);
        }
        void TeleportUp(float amount)
        {
            Vector3 pos = gamebase::natives::transform::GetPlayerWorldPosition();
            pos.Z += amount;
            RequestTeleport(pos);
        }

        void TeleportDown(float amount)
        {
            Vector3 pos = gamebase::natives::transform::GetPlayerWorldPosition();
            pos.Z -= amount;
            RequestTeleport(pos);
        }

        void Tick()
        {
            if (tickTeleportNow)
            {
                gamebase::natives::transform::SetPlayerWorldTransform(pendingTeleportPosition, { pendingTeleportRotation.X, pendingTeleportRotation.Y, pendingTeleportRotation.Z});
                tickTeleportNow = false;
            }
        }

	}
}