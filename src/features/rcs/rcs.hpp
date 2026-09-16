#pragma once

#include <optional>
#include "core/engine/types/vector2.hpp"

class RCS
{
public:
  RCS() = default;

  void update(float dt);

private:
  Vector2 lastDetectedPunch = {0.0f, 0.0f};
  Vector2 currentSimulatedPunch = {0.0f, 0.0f};
  Vector2 accumulatedError = {0.0f, 0.0f};

  float springVelocityX = 0.0f;
  float springVelocityY = 0.0f;

private:
  void resetState(std::optional<Vector2> aimPunch);
};