#include "aim_controller.hpp"

#include "utils/mouse/mouse.hpp"
#include "utils/math/math.hpp"
#include "utils/logger/logger.hpp"

void AimController::update(float dt)
{
  Cache::withLock([this](const Cache &cache)
                  {
    Vector3 cameraPos = cache.localPlayer.cameraPos;

    Vector3 aimAngles = cache.localPlayer.viewAngle;
    aimAngles.z = 0.0f;
    Vector3 rayDir = anglesToForward(aimAngles).normalized();

    for (auto &player : cache.players) 
    {
      if (!player.isAlive || player.teamNum == cache.localPlayer.teamNum) continue;

      bool isValidTarget = false;

      for (int i = 0; i < 19; i++)
      {
        const auto& hb = player.hitboxes[i];
        if (hb.hitbox.shapeRadius <= 0.0f) continue;

        const auto& bone = hb.bone;

        Vector3 scaledMin = hb.hitbox.minBounds * bone.scale;
        Vector3 scaledMax = hb.hitbox.maxBounds * bone.scale;
        float scaledRadius = hb.hitbox.shapeRadius * bone.scale;

        Vector3 worldMin = scaledMin.transformByVector3(bone.rotation) + bone.position;
        Vector3 worldMax = scaledMax.transformByVector3(bone.rotation) + bone.position;

        if (intersectRayCapsule(cameraPos, rayDir, worldMin, worldMax, scaledRadius))
        {
          Vector3 hitboxCenter = (worldMin + worldMax) / 2.0f;

          if (visCheck_.isVisible(cameraPos, hitboxCenter))
          {
            isValidTarget = true;
            break;
          }
        }
      }

      if (isValidTarget) 
      {
        logger::info("VALID TARGET IN CROSSHAIR!");
      }
    } });
}