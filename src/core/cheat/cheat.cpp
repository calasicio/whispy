#include "cheat.hpp"

#include <chrono>
#include <thread>
#include <regex>

#include "core/engine/engine.hpp"
#include "utils/logger/logger.hpp"

bool Cheat::init()
{
  return getInstance().initImpl();
}

void Cheat::thread()
{
  return getInstance().threadImpl();
}

void Cheat::destroy()
{
  return getInstance().destroyImpl();
}

bool Cheat::initImpl()
{
  SetForegroundWindow(Engine::getProcess()->hwnd_);

  logger::info("Succesfully initialized cheat...");

  std::thread(&Cheat::threadImpl, this).detach();

  return true;
}

void Cheat::threadImpl()
{
  auto lastFrame = std::chrono::steady_clock::now();
  std::string lastMap;

  while (isRunning)
  {
    const auto now = std::chrono::steady_clock::now();

    const float dt = std::chrono::duration<float>(now - lastFrame).count();
    lastFrame = now;

    Cache::withLock([this, &lastMap](const Cache &cache)
                    {
      std::string currentMap = cache.globals.mapName;
      if (currentMap != lastMap && !currentMap.empty() && currentMap != "maps/<empty>.vpk") {
        lastMap = currentMap;

        std::regex mapRegex(R"(maps/([^.]+)\.vpk)");
        std::smatch match;

        if (std::regex_search(currentMap, match, mapRegex)) {
          std::string isolatedMapName = match[1].str(); 
          
          visCheck.loadMap(isolatedMapName);
        }
      } });

    aimController.update(dt);
    rcs.update(dt);
    autoStrafe.update(dt);
    autoSwitch.update(dt);

    std::this_thread::sleep_until(now + std::chrono::milliseconds(1));
  }
}

void Cheat::destroyImpl()
{
  isRunning = false;
}
