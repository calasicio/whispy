#include "player.hpp"

#include "core/engine/engine.hpp"
#include "core/offsets/offsets.hpp"

bool Player::update()
{
  if (!Engine::getProcess())
    return false;

  if (!getController())
    return false;

  if (!getPawn())
    return false;

  if (!updatePawn())
    return false;

  return true;
}

bool Player::getController()
{
  auto process = Engine::getProcess();

  this->controller = process->read<DWORD64>(this->listEntry + (this->index + 1) * 0x70);

  return this->controller != 0;
}

bool Player::getPawn()
{
  auto process = Engine::getProcess();
  auto client = Engine::getClient();

  auto entityPawnAddress = process->read<uintptr_t>(this->controller + offsets::player::controller::m_hPawn);

  if (!entityPawnAddress)
    return false;

  auto entityPawnListEntry = process->read<uintptr_t>(this->entityList + 0x10 + 0x8 * ((entityPawnAddress & 0x7FFF) >> 9));

  if (!entityPawnListEntry)
    return false;

  this->pawn = process->read<uintptr_t>(entityPawnListEntry + 0x70 * (entityPawnAddress & 0x1FF));

  return this->pawn != 0;
}

bool Player::updatePawn()
{
  auto process = Engine::getProcess();

  this->teamNum = process->read<uint8_t>(this->pawn + offsets::entities::base::m_iTeamNum);

  this->health = process->read<int>(pawn + offsets::entities::base::m_iHealth);
  this->isAlive = health > 0;

  if (!isAlive)
  {
    origin = Vector3{0, 0, 0};
    velocity = Vector3{0, 0, 0};
    speed = 0;

    shotsFired.old = 0;
    shotsFired.current = 0;

    isDefusing.old = false;
    isDefusing.current = false;

    isGrabbingHostage.old = false;
    isGrabbingHostage.current = false;

    return true;
  }

  this->origin = process->read<Vector3>(this->pawn + offsets::player::pawn::m_vOldOrigin);
  this->velocity = process->read<Vector3>(this->pawn + offsets::entities::base::m_vecAbsVelocity);
  this->speed = this->velocity.length();

  int shotsFired_ = process->read<int>(this->pawn + offsets::player::pawn::m_iShotsFired);
  this->shotsFired.old = this->shotsFired.current;
  this->shotsFired.current = shotsFired_;

  if (!isLocalPlayer)
  {
    this->spottedMask = process->read<uint32_t>(this->pawn + offsets::player::pawn::m_entitySpottedState + offsets::player::pawn::m_bSpottedByMask);

    bool isDefusing_ = process->read<bool>(this->pawn + offsets::player::pawn::m_bIsDefusing);
    this->isDefusing.old = this->isDefusing.current;
    this->isDefusing.current = isDefusing_;

    bool isGrabbingHostage_ = process->read<bool>(this->pawn + offsets::player::pawn::m_bIsGrabbingHostage);
    this->isGrabbingHostage.old = this->isGrabbingHostage.current;
    this->isGrabbingHostage.current = isGrabbingHostage_;

    updateBones();
    // updateHitboxes();
    updateSoundStates();
  }

  return true;
}

/*
 * TODO:
 * - Landing sounds (also somehow handle the silent landing too)
 * - Utility sounds
 *      |-- Get all the utlities thrown by a player,
 *          get the bouncing sound, explosion sound, unpin sound
 */

bool Player::updateSoundStates()
{
  auto process = Engine::getProcess();

  bool isMakingSound = false;

  // DEFUSING
  if (!this->isDefusing.old && this->isDefusing.current)
  {
    this->lastSoundMade.soundName = "DEFUSING";
    this->lastSoundMade.radius = 700;

    isMakingSound = true;
  }

  // GRABBING_HOSTAGE
  if (!this->isGrabbingHostage.old && this->isGrabbingHostage.current)
  {
    this->lastSoundMade.soundName = "GRABBING_HOSTAGE";
    this->lastSoundMade.radius = 700;

    isMakingSound = true;
  }

  // SHOOTING
  if (this->shotsFired.old == 0 && this->shotsFired.current > 0)
  {
    this->lastSoundMade.soundName = "SHOOTING";
    this->lastSoundMade.radius = 999999;

    isMakingSound = true;
  }

  // RUNNING
  if (this->speed >= 135)
  {
    this->lastSoundMade.soundName = "RUNNING";
    this->lastSoundMade.radius = 1000;

    isMakingSound = true;
  }

  if (isMakingSound)
  {
    this->lastSoundMade.timestamp = std::chrono::steady_clock::now();
  }

  return true;
}

bool Player::updateBones()
{
  auto process = Engine::getProcess();

  this->hitboxes.clear();

  uintptr_t gameSceneNode = process->read<uintptr_t>(this->pawn + offsets::entities::base::m_pGameSceneNode);
  if (gameSceneNode)
  {
    uintptr_t boneArray = process->read<uintptr_t>(gameSceneNode + offsets::entities::base::m_modelState + offsets::entities::base::m_boneArray);

    Bone boneBuffer[23];

    if (boneArray)
    {
      if (process->read_raw(boneArray, &boneBuffer, sizeof(boneBuffer)))
      {
        for (int i = 0; i < 23; i++)
        {
          this->bones[i] = boneBuffer[i];

          const CHitbox *hitbox = getBoneHitbox(i);
          if (hitbox)
          {
            this->hitboxes.emplace_back(Hitbox{boneBuffer[i], *hitbox});
          }
        }
      }
    }
  }

  return true;
}

bool alreadyDidIt = false;
bool Player::updateHitboxes()
{
  if (alreadyDidIt)
  {
    return false;
  }

  std::cout << "Getting hitboxes" << std::endl;

  alreadyDidIt = true;

  auto process = Engine::getProcess();

  uintptr_t gameSceneNode = process->read<uintptr_t>(this->pawn + offsets::entities::base::m_pGameSceneNode);
  if (!gameSceneNode)
    return false;

  uintptr_t tempModel = process->read<uintptr_t>(gameSceneNode + offsets::entities::base::m_modelState + offsets::entities::base::m_hModel);
  if (!tempModel)
    return false;

  uintptr_t model = process->read<uintptr_t>(tempModel);
  if (!model)
    return false;

  uintptr_t tempRefMeshes = process->read<uintptr_t>(model + offsets::entities::meshes::m_refMeshes);
  if (!tempRefMeshes)
    return false;

  uintptr_t refMeshes = process->read<uintptr_t>(tempRefMeshes);
  if (!refMeshes)
    return false;

  uintptr_t hitboxData = process->read<uintptr_t>(refMeshes + offsets::entities::meshes::m_hitboxData);
  if (!hitboxData)
    return false;

  int hitboxCount = process->read<int>(hitboxData + offsets::entities::meshes::hitbox::count);
  if (hitboxCount <= 0 || hitboxCount > 64)
    return false;

  uintptr_t hitboxes = process->read<uintptr_t>(hitboxData + offsets::entities::meshes::hitbox::array);
  if (!hitboxes)
    return false;

  for (int i = 0; i < hitboxCount; i++)
  {

    uintptr_t targetHitbox = hitboxes + (offsets::entities::meshes::hitbox::size * i);

    uintptr_t boneNamePtr = process->read<uintptr_t>(targetHitbox + offsets::entities::meshes::hitbox::m_sBoneName);
    std::string boneName = boneNamePtr ? process->read_string(boneNamePtr) : "unknown";

    int boneIndex = getBoneIndex(boneName);

    Bone bone = this->bones[boneIndex];

    Vector3 minBounds = process->read<Vector3>(targetHitbox + offsets::entities::meshes::hitbox::m_vMinBounds);
    Vector3 maxBounds = process->read<Vector3>(targetHitbox + offsets::entities::meshes::hitbox::m_vMaxBounds);
    float shapeRadius = process->read<float>(targetHitbox + offsets::entities::meshes::hitbox::m_flShapeRadius);

    // std::cout << "Hitbox: { bone: {" << bone.position << ", " << bone.rotation << ", " << bone.scale << "}, " << boneName << ", " << minBounds << ", " << maxBounds << ", " << shapeRadius << "}" << std::endl;
    std::cout << "Hitbox: " << boneName << "(" << boneIndex << "), " << minBounds << ", " << maxBounds << ", " << shapeRadius << "}" << std::endl;
  }

  return true;
}