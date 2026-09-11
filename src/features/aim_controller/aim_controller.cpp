#include "aim_controller.hpp"

#include "utils/logger/logger.hpp"

void AimController::update(float dt)
{
  Cache::withLock([this](const Cache &cache)
                  {
    for (auto &player : cache.players) {
      if (!player.isAlive || player.teamNum == cache.localPlayer.teamNum) {
        continue;
      }

      bool isVisible = visCheck_.isVisible(cache.localPlayer.cameraPos, player.bones.head);

      if (isVisible) {
        logger::info("Visible");
      }
    } });
}
