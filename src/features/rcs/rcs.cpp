#include "rcs.hpp"
#include "rcs_constants.hpp"

#include <cmath>
#include <algorithm>

#include "core/engine/cache/cache.hpp"
#include "utils/mouse/mouse.hpp"
#include "utils/random/random.hpp"

void RCS::update(float dt)
{
  (void)dt;

  Cache::withLock([this](const Cache &cache)
                  {
    if (!cache.localPlayer.isAlive)
    {
      resetState(std::nullopt);
      return;
    }

    Vector3 aimPunch = cache.localPlayer.aimPunch;
    Vector2 currentAimPunch = {aimPunch.x, aimPunch.y};
    int currentShots = cache.localPlayer.shotsFired.current;

    if (currentShots <= 1)
    {
      resetState(currentAimPunch);
      return;
    }

    if (currentAimPunch.x != lastDetectedPunch.x || currentAimPunch.y != lastDetectedPunch.y)
    {
      lastDetectedPunch = currentAimPunch;
    }
    Vector2 targetPunch = lastDetectedPunch;

    float pullFactorX = 0.0f;
    float frictionX = 0.0f;
    float pullFactorY = 0.0f;
    float frictionY = 0.0f;

    if (currentShots <= 10)
    {
      pullFactorX = 0.15f; frictionX = 0.60f;
      pullFactorY = 0.15f; frictionY = 0.60f;
    }
    else
    {
      pullFactorX = 0.04f; frictionX = 0.90f;

      if (std::abs(targetPunch.y - currentSimulatedPunch.y) < 0.03f) {
        pullFactorY = 0.0f; 
        frictionY = 0.50f;
      } else {
        pullFactorY = 0.08f; 
        frictionY = 0.75f;
      }
    }

    springVelocityX = (springVelocityX + (targetPunch.x - currentSimulatedPunch.x) * pullFactorX) * frictionX;
    springVelocityY = (springVelocityY + (targetPunch.y - currentSimulatedPunch.y) * pullFactorY) * frictionY;

    Vector2 nextSimulatedPunch = {
      currentSimulatedPunch.x + springVelocityX,
      currentSimulatedPunch.y + springVelocityY
    };

    if (currentShots <= 8)
    {
      nextSimulatedPunch.x += random::generateGaussianNoise(0.0f, 0.0002f);
      nextSimulatedPunch.y += random::generateGaussianNoise(0.0f, 0.0001f);
    }
    else
    {
      nextSimulatedPunch.x += random::generateGaussianNoise(0.0f, 0.0008f);
      nextSimulatedPunch.y += random::generateGaussianNoise(0.0f, 0.0004f);
    }

    Vector2 deltaPunch = (nextSimulatedPunch - currentSimulatedPunch);
    deltaPunch = -deltaPunch * 2.0f;

    Vector2 moveAmount = {
        (deltaPunch.y / (cache.convars.sensitivity * -YAW_PITCH_FACTOR)) + accumulatedError.x,
        (deltaPunch.x / (cache.convars.sensitivity * YAW_PITCH_FACTOR)) + accumulatedError.y};

    int moveX = static_cast<int>(moveAmount.x);
    int moveY = static_cast<int>(moveAmount.y);

    accumulatedError.x = moveAmount.x - moveX;
    accumulatedError.y = moveAmount.y - moveY;

    if (moveX != 0 || moveY != 0)
      mouse::moveMouseRelative(moveX, moveY);

    currentSimulatedPunch = nextSimulatedPunch; });
}

void RCS::resetState(std::optional<Vector2> aimPunch)
{
  if (aimPunch.has_value())
  {
    lastDetectedPunch = aimPunch.value();
    currentSimulatedPunch = aimPunch.value();
  }
  else
  {
    lastDetectedPunch = {0.0f, 0.0f};
    currentSimulatedPunch = {0.0f, 0.0f};
  }

  springVelocityX = 0.0f;
  springVelocityY = 0.0f;
  accumulatedError = {0.0f, 0.0f};
}