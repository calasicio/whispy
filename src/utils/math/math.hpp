#pragma once

#include <cmath>

#include <imgui.h>
#include "core/engine/types/vector2.hpp"
#include "core/engine/types/vector3.hpp"
#include "core/engine/types/structs.hpp"

inline Vector3 anglesToForward(const Vector3 &angles)
{
  float pitchRad = angles.x * (3.14159265358979323846f / 180.0f);
  float yawRad = angles.y * (3.14159265358979323846f / 180.0f);

  float cp = std::cos(pitchRad);
  float sp = std::sin(pitchRad);
  float cy = std::cos(yawRad);
  float sy = std::sin(yawRad);

  Vector3 forward;
  forward.x = cp * cy;
  forward.y = cp * sy;
  forward.z = -sp;

  return forward;
}

inline bool worldToScreen(const Vector3 &worldPos, ImVec2 &screenPos, const ViewMatrix &matrix, const ImVec2 &displaySize)
{
  float w = worldPos.x * matrix.m[3][0] + worldPos.y * matrix.m[3][1] + worldPos.z * matrix.m[3][2] + matrix.m[3][3];

  if (w < 0.001f)
    return false;

  float x = worldPos.x * matrix.m[0][0] + worldPos.y * matrix.m[0][1] + worldPos.z * matrix.m[0][2] + matrix.m[0][3];
  float y = worldPos.x * matrix.m[1][0] + worldPos.y * matrix.m[1][1] + worldPos.z * matrix.m[1][2] + matrix.m[1][3];

  float ndcX = x / w;
  float ndcY = y / w;

  screenPos.x = (displaySize.x * 0.5f) + (ndcX * displaySize.x * 0.5f);
  screenPos.y = (displaySize.y * 0.5f) - (ndcY * displaySize.y * 0.5f);

  return true;
}

inline bool intersectRayCapsule(const Vector3 &rayOrigin, const Vector3 &rayDir, const Vector3 &minBounds, const Vector3 &maxBounds, float radius)
{
  Vector3 d1 = rayDir;
  Vector3 d2 = maxBounds - minBounds;
  Vector3 w0 = rayOrigin - minBounds;

  float a = Vector3::dot(d1, d1);
  float b = Vector3::dot(d1, d2);
  float c = Vector3::dot(d1, w0);
  float d = Vector3::dot(d2, d2);
  float e = Vector3::dot(d2, w0);

  float denom = a * d - b * b;
  float s = 0.0f, t = 0.0f;

  if (std::abs(denom) < 0.0001f)
  {
    s = 0.0f;
    t = (b > d ? e / b : e / d);
  }
  else
  {
    s = (b * e - c * d) / denom;
    t = (a * e - b * c) / denom;
  }

  if (t < 0.0f)
  {
    t = 0.0f;
    s = -c / a;
  }
  else if (t > 1.0f)
  {
    t = 1.0f;
    s = (b - c) / a;
  }

  if (s < 0.0f)
  {
    s = 0.0f;
    t = e / d;
    if (t < 0.0f)
      t = 0.0f;
    else if (t > 1.0f)
      t = 1.0f;
  }

  Vector3 c1 = rayOrigin + (d1 * s);
  Vector3 c2 = minBounds + (d2 * t);

  return c1.distance(c2) <= radius;
}