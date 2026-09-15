#pragma once

#include <cstdint>
#include <chrono>
#include <string>
#include <map>

#include "core/engine/types/structs.hpp"
#include "core/engine/types/vector2.hpp"
#include "core/engine/types/vector3.hpp"

template <class T>
struct StateHistory
{
  T old;
  T current;
};

struct SoundEvent
{
  std::string soundName;
  float radius;
  std::chrono::steady_clock::time_point timestamp;
};

class Player
{
public:
  Player()
  {
    this->hitboxes.reserve(19);
  }
  Player(int index, uintptr_t entityList, uintptr_t listEntry)
      : index(index), entityList(entityList), listEntry(listEntry)
  {
    this->hitboxes.reserve(19);
  }

  virtual ~Player() = default;

  virtual bool update();

public:
  int8_t index = -1;
  bool isLocalPlayer = false;

  std::uintptr_t pawn;
  std::uintptr_t controller;

  std::vector<Hitbox> hitboxes;
  Bone bones[23];

  int health;
  bool isAlive;
  int teamNum;

  Vector3 origin;
  Vector3 velocity;
  float speed;

  uint32_t spottedMask;
  StateHistory<bool> isShooting = {false, false};
  StateHistory<bool> isDefusing = {false, false};
  StateHistory<bool> isGrabbingHostage = {false, false};
  StateHistory<int> shotsFired = {0, 0};

  SoundEvent lastSoundMade;
  std::chrono::steady_clock::time_point lastHeard;
  bool isShownInRadar = false;

private:
  uintptr_t entityList;
  uintptr_t listEntry;

private:
  bool getPawn();
  bool getController();

  bool updateSoundStates();
  bool updateBones();
  bool updateHitboxes();

protected:
  virtual bool updatePawn();
};

inline const CHitbox *getBoneHitbox(int boneId)
{
  static const std::map<int, CHitbox> boneIdToHitbox = {
      {7, {{-1, 1.8, 0}, {3.5, 0.2, 0}, 4.3f}},                 // head_0
      {6, {{0, -0.4, 0}, {1.4, -0.2, 0}, 3.5f}},                // neck_0
      {1, {{-2.7, 1.1, -3.2}, {-2.7, 1.1, 3.2}, 6.0f}},         // pelvis
      {2, {{1.4, 0.8, 3.1}, {1.4, 0.8, -3.1}, 6.0f}},           // spine_0
      {3, {{3.8, 0.8, -2.4}, {3.8, 0.4, 2.4}, 6.5f}},           // spine_1
      {4, {{4.8, 0.15, -4.1}, {4.8, 0.15, 4.1}, 6.2f}},         // spine_2
      {5, {{2.5, -0.6, -6}, {2.5, -0.6, 6}, 5.0f}},             // spine_3
      {17, {{1.3, -0.2, 0}, {16.5, -0.7, 0}, 5.0f}},            // leg_upper_l
      {20, {{-1.3, 0, -0.6}, {-16.5, 0, -0.7}, 5.0f}},          // leg_upper_r
      {18, {{0.1, -0.4, 0.2}, {17, -0.4, 0.7}, 4.0f}},          // leg_lower_l
      {21, {{-0.1, 0, -0.2}, {-17, 0.4, -0.7}, 4.0f}},          // leg_lower_r
      {19, {{-0.0, -3.43, -0.52}, {8.0, 0.74, 0.33}, 2.6f}},    // ankle_l
      {22, {{-7.98, -0.75, -0.27}, {-0.02, 3.44, 0.58}, 2.6f}}, // ankle_r
      {11, {{0, 0.3, 0}, {3.59, 1.15, 0.11}, 2.3f}},            // hand_l
      {15, {{0, -0.3, 0.02}, {-3.44, -1.17, -0.09}, 2.3f}},     // hand_r
      {9, {{0, 0, 0}, {11.2, 0, 0}, 3.3f}},                     // arm_upper_l
      {10, {{0, 0, 0}, {10, 0, 0}, 3.0f}},                      // arm_lower_l
      {13, {{0, 0, 0}, {-11.2, 0, 0}, 3.3f}},                   // arm_upper_r
      {14, {{0, 0, 0}, {-10, 0, -0.5}, 3.0f}}                   // arm_lower_r
  };

  auto it = boneIdToHitbox.find(boneId);
  if (it != boneIdToHitbox.end())
  {
    return &it->second;
  }

  return nullptr;
}

inline int getBoneIndex(const std::string &key)
{
  const std::map<std::string, int> boneNameToId = {
      {"head_0", 7},
      {"neck_0", 6},
      {"pelvis", 1},
      {"spine_0", 2},
      {"spine_1", 3},
      {"spine_2", 4},
      {"spine_3", 5},
      {"leg_upper_l", 17},
      {"leg_upper_r", 20},
      {"leg_lower_l", 18},
      {"leg_lower_r", 21},
      {"ankle_l", 19},
      {"ankle_r", 22},
      {"hand_l", 11},
      {"hand_r", 15},
      {"arm_upper_l", 9},
      {"arm_lower_l", 10},
      {"arm_upper_r", 13},
      {"arm_lower_r", 14},
  };

  auto it = boneNameToId.find(key);
  if (it != boneNameToId.end())
  {
    return it->second;
  }

  return -1;
}
