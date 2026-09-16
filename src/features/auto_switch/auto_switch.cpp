#include "auto_switch.hpp"

#include "utils/keys/keys.hpp"
#include "utils/mouse/mouse.hpp"

void AutoSwitch::update(float dt)
{
  Cache::withLock([this, dt](const Cache &cache)
                  {
    if (!cache.localPlayer.isAlive || cache.localPlayer.shotsFired.current < 1)
      return;

    if (cache.localPlayer.currentWeaponId != 9 && cache.localPlayer.currentWeaponId != 40)
      return;
    
    std::thread([]() 
    {
      keys::sendKeyDown('3');
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      keys::sendKeyUp('3');
      
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      
      keys::sendKeyDown('1');
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      keys::sendKeyUp('1');
    }).detach();
  });
}