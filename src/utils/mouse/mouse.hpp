#pragma once

#include <Windows.h>

namespace mouse
{
  inline void moveMouseRelative(int x, int y)
  {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = x;
    input.mi.dy = y;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
  }

  inline bool isButtonPressed(int button)
  {
    int vKey = 0;

    switch (button)
    {
    case 1:
      vKey = VK_LBUTTON;
      break;
    case 2:
      vKey = VK_RBUTTON;
      break;
    case 3:
      vKey = VK_MBUTTON;
      break;
    case 4:
      vKey = VK_XBUTTON1;
      break;
    case 5:
      vKey = VK_XBUTTON2;
      break;
    default:
      return false;
    }

    return (GetAsyncKeyState(vKey) & 0x8000) != 0;
  }

  inline void setButtonDown(int button)
  {
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    switch (button)
    {
    case 1:
      input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
      break;
    case 2:
      input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
      break;
    case 3:
      input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
      break;
    case 4:
      input.mi.dwFlags = MOUSEEVENTF_XDOWN;
      input.mi.mouseData = XBUTTON1;
      break;
    case 5:
      input.mi.dwFlags = MOUSEEVENTF_XDOWN;
      input.mi.mouseData = XBUTTON2;
      break;
    default:
      return;
    }

    SendInput(1, &input, sizeof(INPUT));
  }

  inline void setButtonUp(int button)
  {
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    switch (button)
    {
    case 1:
      input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
      break;
    case 2:
      input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
      break;
    case 3:
      input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
      break;
    case 4:
      input.mi.dwFlags = MOUSEEVENTF_XUP;
      input.mi.mouseData = XBUTTON1;
      break;
    case 5:
      input.mi.dwFlags = MOUSEEVENTF_XUP;
      input.mi.mouseData = XBUTTON2;
      break;
    default:
      return;
    }

    SendInput(1, &input, sizeof(INPUT));
  }

  inline void clickButton(int button)
  {
    setButtonDown(button);
    setButtonUp(button);
  }
}