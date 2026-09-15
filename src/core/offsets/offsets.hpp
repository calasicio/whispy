#pragma once

#include <cstdint>

namespace offsets
{
  namespace globals
  {
    // offsets.dll -> [client_dll]
    inline std::ptrdiff_t dwGlobalVars = 0x20AF5F0;
    inline std::ptrdiff_t m_iMaxClients = 0x10;
    inline std::ptrdiff_t m_strMapName = 0x180;
  }

  namespace game
  {
    // offsets.dll -> [client_dll]
    inline std::ptrdiff_t dwViewMatrix = 0x23CB830;
    // offsets.dll -> [client_dll]
    inline std::ptrdiff_t dwViewRender = 0x23CB898;
    inline std::ptrdiff_t CCVars;

    // offsets.dll -> [engine2_dll]
    inline std::ptrdiff_t dwWindowHeight = 0x9118D4;
    // offsets.dll -> [engine2_dll]
    inline std::ptrdiff_t dwWindowWidth = 0x9118D0;
  }

  namespace entities
  {
    // offsets.dll -> [client_dll]
    inline std::ptrdiff_t dwEntityList = 0x2571220;

    namespace base
    {
      // client.dll -> [client_dll] -> [C_BaseEntity]
      inline std::ptrdiff_t m_pGameSceneNode = 0x330;

      // client.dll -> [client_dll] -> [C_BaseEntity]
      inline std::ptrdiff_t m_iHealth = 0x34C;
      // client.dll -> [client_dll] -> [C_BaseEntity]
      inline std::ptrdiff_t m_nSubclassID = 0x380;
      // client.dll -> [client_dll] -> [C_BaseEntity]
      inline std::ptrdiff_t m_iTeamNum = 0x3E7;
      // client.dll -> [client_dll] -> [C_BaseEntity]
      inline std::ptrdiff_t m_fFlags = 0x3F4;
      // client.dll -> [client_dll] -> [C_BaseEntity]
      inline std::ptrdiff_t m_vecAbsVelocity = 0x3F8;
      // client.dll -> [client_dll] -> [C_BaseEntity]
      inline std::ptrdiff_t m_MoveType = 0x525;

      // client.dll -> [client_dll] -> [C_BaseModelEntity]
      inline std::ptrdiff_t m_vecViewOffset = 0xE78;

      // client.dll -> [client_dll] -> [CGameSceneNode]
      inline std::ptrdiff_t m_vecOrigin = 0x80;
      // client.dll -> [client_dll] -> [CSkeletonInstance]
      inline std::ptrdiff_t m_modelState = 0x140;
      inline std::ptrdiff_t m_boneArray = 0x80;

      // client.dll -> [client_dll] -> [CModelState]
      inline std::ptrdiff_t m_hModel = 0xA0;

      // client.dll -> [client_dll] -> [CCSWeaponBaseVData]
      inline std::ptrdiff_t m_flMaxSpeed = 0x750;
    }

    namespace meshes
    {
      // animationsystem.dll -> [animationsystem_dll] -> [PermModelData_t]
      inline std::ptrdiff_t m_refMeshes = 0x78;
      // Bruteforce
      inline std::ptrdiff_t m_hitboxData = 0x168;

      namespace hitbox
      {
        inline std::ptrdiff_t array = 0x30;
        inline std::ptrdiff_t count = 0x28;
        inline std::ptrdiff_t size = 0x70;

        // animationsystem.dll -> [animationsystem_dll] -> [CHitBox]
        inline std::ptrdiff_t m_name = 0x0;
        // animationsystem.dll -> [animationsystem_dll] -> [CHitBox]
        inline std::ptrdiff_t m_sBoneName = 0x10;
        // animationsystem.dll -> [animationsystem_dll] -> [CHitBox]
        inline std::ptrdiff_t m_vMinBounds = 0x18;
        // animationsystem.dll -> [animationsystem_dll] -> [CHitBox]
        inline std::ptrdiff_t m_vMaxBounds = 0x24;
        // animationsystem.dll -> [animationsystem_dll] -> [CHitBox]
        inline std::ptrdiff_t m_flShapeRadius = 0x30;
        // animationsystem.dll -> [animationsystem_dll] -> [CHitBox]
        inline std::ptrdiff_t m_nHitBoxIndex = 0x48;
      }
    }
  }

  namespace player
  {
    namespace localplayer
    {
      // offsets.dll -> [client_dll]
      inline std::ptrdiff_t dwLocalPlayerController = 0x23A0F30;
      // offsets.dll -> [client_dll]
      inline std::ptrdiff_t dwLocalPlayerPawn = 0x23C6268;
      // offsets.dll -> [client_dll]
      inline std::ptrdiff_t dwViewAngles = 0x23DC2F8;
    }

    namespace controller
    {
      // client.dll -> [client_dll] -> [CBasePlayerController]
      inline std::ptrdiff_t m_hPawn = 0x6BC;
      // client.dll -> [client_dll] -> [CBasePlayerController]
      inline std::ptrdiff_t m_bIsLocalPlayerController = 0x788;
    }

    namespace pawn
    {
      // client.dll -> [client_dll] -> [C_BasePlayerPawn]
      inline std::ptrdiff_t m_vOldOrigin = 0x13B8;

      // client.dll -> [client_dll] -> [C_CSPlayerPawn]
      inline std::ptrdiff_t m_pAimPunchServices = 0x14B8;
      // client.dll -> [client_dll] -> [C_CSPlayerPawn]
      inline std::ptrdiff_t m_entitySpottedState = 0x1C60;
      // client.dll -> [client_dll] -> [C_CSPlayerPawn]
      inline std::ptrdiff_t m_bIsDefusing = 0x1C7A;
      // client.dll -> [client_dll] -> [C_CSPlayerPawn]
      inline std::ptrdiff_t m_bIsGrabbingHostage = 0x1C7B;
      // client.dll -> [client_dll] -> [C_CSPlayerPawn]
      inline std::ptrdiff_t m_iShotsFired = 0x1C8C;

      // client.dll -> [client_dll] -> [EntitySpottedState_t]
      inline std::ptrdiff_t m_bSpotted = 0x8;
      // client.dll -> [client_dll] -> [EntitySpottedState_t]
      inline std::ptrdiff_t m_bSpottedByMask = 0x8;
    }
  }

  namespace weapon
  {
    // client.dll -> [client_dll] -> [C_BasePlayerPawn]
    inline std::ptrdiff_t m_pWeaponServices = 0x1208;
    // client.dll -> [client_dll] -> [CPlayer_WeaponServices]
    inline std::ptrdiff_t m_hActiveWeapon = 0x60;
    // client.dll -> [client_dll] -> [C_EconEntity]
    inline std::ptrdiff_t m_AttributeManager = 0x11A8;
    // client.dll -> [client_dll] -> [C_AttributeContainer]
    inline std::ptrdiff_t m_Item = 0x50;
    // client.dll -> [client_dll] -> [C_EconItemView]
    inline std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA;
  }

  namespace hud
  {
    inline std::ptrdiff_t cHud = 0x2436710;

    namespace CCSGO_HudTree
    {
      constexpr std::uintptr_t Base = 0x258;  // DAT_182436710 + 600 (600 dec = 0x258)
      constexpr std::uintptr_t Count = 0x264; // *(uint *)(DAT_182436710 + 0x264)
      constexpr std::uintptr_t Pool = 0x268;  // *(longlong *)(DAT_182436710 + 0x268)
    }

    // Just decompile the game and look at the CCSGO_HudRadar struct, the offsets are there
    namespace CCSGO_HudRadar
    {
      constexpr std::uintptr_t IsRound = 0x60;
      constexpr std::uintptr_t MapTexturePosition = 0x190;
      constexpr std::uintptr_t VisibilitySizeMax = 0x19C;
      constexpr std::uintptr_t VisibilitySize = 0x1A0;
      constexpr std::uintptr_t MapTextureScale = 0x1B4;
      constexpr std::uintptr_t MaxVisibilitySquared = 0x1B8;
      constexpr std::uintptr_t OriginTextureDiff = 0x1D0;
      constexpr std::uintptr_t RadarScale = 0x17F8C;
    }
  }
}