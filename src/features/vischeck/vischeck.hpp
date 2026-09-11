#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "core/engine/types/vector3.hpp"

struct TriBoundingBox
{
  Vector3 min, max;
  bool intersect(const Vector3 &ray_origin, const Vector3 &ray_end) const;
};

struct TriTriangle
{
  Vector3 p1, p2, p3;
  bool intersect(const Vector3 &ray_origin, const Vector3 &ray_end) const;
};

struct TriKDNode
{
  TriBoundingBox bbox;
  std::vector<TriTriangle> triangles;
  TriKDNode *left = nullptr;
  TriKDNode *right = nullptr;
  int axis = 0;

  void destroy();
};

class VisCheck
{
public:
  VisCheck() = default;
  ~VisCheck();

  VisCheck(const VisCheck &) = delete;
  VisCheck &operator=(const VisCheck &) = delete;

  bool loadMap(const std::string &mapName);
  void unload();

  bool isVisible(const Vector3 &from, const Vector3 &to) const;
  bool isLoaded() const { return kdTree != nullptr; }

private:
  TriKDNode *kdTree = nullptr;
  std::string currentMap;

  static TriKDNode *buildTree(std::vector<TriTriangle> &triangles, int depth = 0);
  static bool rayHitsTree(TriKDNode *node, const Vector3 &origin, const Vector3 &end);
};