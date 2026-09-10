#pragma once

#include <cstdint>
#include <chrono>

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

struct BoneData
{
  Vector3 head;
  Vector3 neck;
  Vector3 pelvis;
};

class Player
{
public:
  Player() {}
  Player(int index, uintptr_t entityList, uintptr_t listEntry)
      : index(index), entityList(entityList), listEntry(listEntry) {}

  virtual ~Player() = default;

  virtual bool update();

public:
  int8_t index = -1;

  std::uintptr_t pawn;
  std::uintptr_t controller;

  BoneData bones;
  enum BoneIndex : int
  {
    HEAD = 7,
    NECK = 6,
    PELVIS = 1,
  };

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

protected:
  virtual bool updatePawn();
};
