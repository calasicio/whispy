#include "aim_controller.hpp"
#include "aim_controller_constants.hpp"

#include "utils/mouse/mouse.hpp"
#include "utils/random/random.hpp"
#include "utils/math/math.hpp"
#include "utils/logger/logger.hpp"

void AimController::update(float dt)
{
  Cache::withLock([this](const Cache &cache)
                  {
    if (!mouse::isButtonPressed(5))
    {
      if (isShooting)
      {
        mouse::setButtonUp(1);
        isShooting = false;
      }

      hasTarget = false;
      isClickPending = false;

      return;
    }

    bool isAccurate = true;

    {
      if (!cache.localPlayer.isOnGround || cache.localPlayer.moveType != 2) 
      {
        isAccurate = false;
      }
      else if (cache.localPlayer.currentWeaponId == 9 || cache.localPlayer.currentWeaponId == 40) 
      {
        if (!cache.localPlayer.isScoped || cache.localPlayer.currentWeaponInnacuracy > 0.01f)
        {
          isAccurate = false;
        }
      }

      if (isAccurate)
      {
        float accuracyThreshold = cache.localPlayer.maxMovementSpeed * 0.34f;
        
        if (cache.localPlayer.speed > accuracyThreshold) 
        {
          isAccurate = false;
        }
      }

      if (!isAccurate)
      {
        hasTarget = false;
        isClickPending = false;
        return; 
      }
    }

    Vector3 cameraPos = cache.localPlayer.cameraPos;
    Vector3 aimPunch = cache.localPlayer.aimPunch * 2;
    Vector3 aimAngles = cache.localPlayer.viewAngle;
    
    aimAngles -= aimPunch;
    aimAngles.z = 0.0f;

    Vector3 rayDir = anglesToForward(aimAngles).normalized();

    bool shouldShoot = false;
    for (auto &player : cache.players)
    {
      if (!player.isAlive || player.teamNum == cache.localPlayer.teamNum)
        continue;

      bool isValidTarget = false;

      for (int i = 0; i < 19; i++)
      {
        const auto &hb = player.hitboxes[i];
        if (hb.hitbox.shapeRadius <= 0.0f)
          continue;

        const auto &bone = hb.bone;

        Vector3 scaledMin = hb.hitbox.minBounds * bone.scale;
        Vector3 scaledMax = hb.hitbox.maxBounds * bone.scale;
        float scaledRadius = hb.hitbox.shapeRadius * bone.scale;

        Vector3 worldMin = scaledMin.transformByVector3(bone.rotation) + bone.position;
        Vector3 worldMax = scaledMax.transformByVector3(bone.rotation) + bone.position;

        Vector3 exactHitPoint;

        if (intersectRayCapsule(cameraPos, rayDir, worldMin, worldMax, scaledRadius, exactHitPoint))
        {
          if (visCheck_.isVisible(cameraPos, exactHitPoint))
          {
            isValidTarget = true;
            break;
          }
        }
      }

      if (isValidTarget)
      {
        shouldShoot = true;
        break;
      }
    }

    auto now = std::chrono::steady_clock::now();
    
    if (shouldShoot)
    {
      if (!hasTarget) 
      {
          hasTarget = true;
          firstTargetTime = now;
          
          activeTriggerDelay = random::rangeInt(TRIGGER_DELAY_MIN_MS, TRIGGER_DELAY_MAX_MS);

          isClickPending = true;
      }

      lastTargetTime = now; 
    }
    else
    {
      if (hasTarget) 
      {
        hasTarget = false;
      }
    }
  
    if (isClickPending)
    {
      auto elapsedAcquire = std::chrono::duration_cast<std::chrono::milliseconds>(now - firstTargetTime).count();
      
      if (elapsedAcquire >= activeTriggerDelay) 
      {
        mouse::setButtonDown(1);
        isShooting = true;
        isClickPending = false;
        
        activeReleaseDelay = random::rangeInt(RELEASE_DELAY_MIN_MS, RELEASE_DELAY_MAX_MS);

        if (!shouldShoot) {
          lastTargetTime = now;
        }
      }
    }

    if (isShooting && !shouldShoot)
    {
      auto elapsedLost = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTargetTime).count();
      
      if (elapsedLost >= activeReleaseDelay) 
      {
        mouse::setButtonUp(1);
        isShooting = false;
      }
    }
  });
}