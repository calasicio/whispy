#include "vischeck.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>

#include "utils/logger/logger.hpp"

bool TriBoundingBox::intersect(const Vector3 &ray_origin, const Vector3 &ray_end) const
{
  Vector3 dir = ray_end - ray_origin;

  float invX = 1.0f / (dir.x + (dir.x == 0.0f ? 1e-8f : 0.0f));
  float invY = 1.0f / (dir.y + (dir.y == 0.0f ? 1e-8f : 0.0f));
  float invZ = 1.0f / (dir.z + (dir.z == 0.0f ? 1e-8f : 0.0f));

  float tx1 = (min.x - ray_origin.x) * invX;
  float tx2 = (max.x - ray_origin.x) * invX;
  float ty1 = (min.y - ray_origin.y) * invY;
  float ty2 = (max.y - ray_origin.y) * invY;
  float tz1 = (min.z - ray_origin.z) * invZ;
  float tz2 = (max.z - ray_origin.z) * invZ;

  float tmin = std::max({std::min(tx1, tx2), std::min(ty1, ty2), std::min(tz1, tz2)});
  float tmax = std::min({std::max(tx1, tx2), std::max(ty1, ty2), std::max(tz1, tz2)});

  return tmax >= 0.0f && tmin <= tmax && tmin <= 1.0f;
}

bool TriTriangle::intersect(const Vector3 &ray_origin, const Vector3 &ray_end) const
{
  constexpr float EPSILON = 1e-7f;
  Vector3 dir = ray_end - ray_origin;

  Vector3 edge1 = p2 - p1;
  Vector3 edge2 = p3 - p1;
  Vector3 h = Vector3::cross(dir, edge2);
  float a = edge1.dot(h);

  if (a > -EPSILON && a < EPSILON)
    return false;

  float f = 1.0f / a;
  Vector3 s = ray_origin - p1;
  float u = f * s.dot(h);
  if (u < 0.0f || u > 1.0f)
    return false;

  Vector3 q = Vector3::cross(s, edge1);
  float v = f * dir.dot(q);
  if (v < 0.0f || u + v > 1.0f)
    return false;

  float t = f * edge2.dot(q);
  return t > EPSILON && t < 1.0f;
}

void TriKDNode::destroy()
{
  if (left)
  {
    left->destroy();
    delete left;
  }
  if (right)
  {
    right->destroy();
    delete right;
  }
}

VisCheck::~VisCheck()
{
  unload();
}

void VisCheck::unload()
{
  if (kdTree)
  {
    kdTree->destroy();
    delete kdTree;
    kdTree = nullptr;
  }
  currentMap.clear();
}

bool VisCheck::loadMap(const std::string &mapName)
{
  unload();

  auto begin = std::chrono::steady_clock::now();

  std::string clean = mapName;
  std::transform(clean.begin(), clean.end(), clean.begin(), ::tolower);

  std::string path = "tri/" + clean + ".tri";

  std::ifstream in(path, std::ios::binary | std::ios::ate);
  if (!in.is_open())
  {
    path = clean + ".tri";
    in.open(path, std::ios::binary | std::ios::ate);
    if (!in.is_open())
    {
      logger::warning("VisCheck: could not find .tri for map: " + clean);
      return false;
    }
  }

  std::streamsize fileSize = in.tellg();
  in.seekg(0, std::ios::beg);

  if (fileSize == 0 || fileSize % sizeof(TriTriangle) != 0)
  {
    logger::error("VisCheck: invalid .tri file size: " + std::to_string(fileSize));
    return false;
  }

  size_t numTris = static_cast<size_t>(fileSize) / sizeof(TriTriangle);
  std::vector<TriTriangle> triangles(numTris);

  if (!in.read(reinterpret_cast<char *>(triangles.data()), fileSize))
  {
    logger::error("VisCheck: failed to read .tri file");
    return false;
  }
  in.close();

  logger::info("VisCheck: building KD-tree for " + clean + " (" + std::to_string(numTris) + " triangles)...");

  kdTree = buildTree(triangles);

  auto elapsed = std::chrono::duration<double, std::milli>(
                     std::chrono::steady_clock::now() - begin)
                     .count();

  if (!kdTree)
  {
    logger::error("VisCheck: failed to build KD-tree");
    return false;
  }

  currentMap = clean;
  logger::info("VisCheck: loaded " + clean + " in " + std::to_string(elapsed) + "ms");
  return true;
}

TriKDNode *VisCheck::buildTree(std::vector<TriTriangle> &triangles, int depth)
{
  if (triangles.empty())
    return nullptr;

  TriKDNode *node = new TriKDNode();
  node->axis = depth % 3;

  // Compute bbox
  node->bbox.min = node->bbox.max = triangles[0].p1;
  for (const auto &tri : triangles)
  {
    for (const auto &p : {tri.p1, tri.p2, tri.p3})
    {
      node->bbox.min.x = std::min(node->bbox.min.x, p.x);
      node->bbox.min.y = std::min(node->bbox.min.y, p.y);
      node->bbox.min.z = std::min(node->bbox.min.z, p.z);
      node->bbox.max.x = std::max(node->bbox.max.x, p.x);
      node->bbox.max.y = std::max(node->bbox.max.y, p.y);
      node->bbox.max.z = std::max(node->bbox.max.z, p.z);
    }
  }

  if (triangles.size() <= 4)
  {
    node->triangles = std::move(triangles);
    return node;
  }

  // Median split on current axis
  int axis = node->axis;
  auto comparator = [axis](const TriTriangle &a, const TriTriangle &b)
  {
    auto center = [axis](const TriTriangle &t)
    {
      if (axis == 0)
        return (t.p1.x + t.p2.x + t.p3.x) / 3.0f;
      if (axis == 1)
        return (t.p1.y + t.p2.y + t.p3.y) / 3.0f;
      return (t.p1.z + t.p2.z + t.p3.z) / 3.0f;
    };
    return center(a) < center(b);
  };

  size_t mid = triangles.size() / 2;
  std::nth_element(triangles.begin(), triangles.begin() + mid, triangles.end(), comparator);

  std::vector<TriTriangle> leftTris(triangles.begin(), triangles.begin() + mid);
  std::vector<TriTriangle> rightTris(triangles.begin() + mid, triangles.end());

  node->left = buildTree(leftTris, depth + 1);
  node->right = buildTree(rightTris, depth + 1);

  return node;
}

bool VisCheck::rayHitsTree(TriKDNode *node, const Vector3 &origin, const Vector3 &end)
{
  if (!node)
    return false;
  if (!node->bbox.intersect(origin, end))
    return false;

  // Leaf
  if (!node->triangles.empty())
  {
    for (const auto &tri : node->triangles)
    {
      if (tri.intersect(origin, end))
        return true;
    }
    return false;
  }

  if (rayHitsTree(node->left, origin, end))
    return true;
  return rayHitsTree(node->right, origin, end);
}

bool VisCheck::isVisible(const Vector3 &from, const Vector3 &to) const
{
  if (!kdTree)
    return false;
  return !rayHitsTree(kdTree, from, to);
}