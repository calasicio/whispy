#pragma once

#include "core/engine/cache/cache.hpp"
#include "features/vischeck/vischeck.hpp"

class AimController
{
public:
  explicit AimController(VisCheck &visCheck) : visCheck_(visCheck) {}

  void update(float dt);

private:
  VisCheck &visCheck_;
};