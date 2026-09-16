#pragma once

#include <chrono>

#include "core/engine/cache/cache.hpp"
#include "features/vischeck/vischeck.hpp"

class AimController
{
public:
  explicit AimController(VisCheck &visCheck) : visCheck_(visCheck) {}

  void update(float dt);

private:
  VisCheck &visCheck_;

private:
  bool isShooting = false;
  bool isClickPending = false;
  bool hasTarget = false;
  std::chrono::steady_clock::time_point firstTargetTime;
  std::chrono::steady_clock::time_point lastTargetTime;

  int activeTriggerDelay = 0;
  int activeReleaseDelay = 0;
};