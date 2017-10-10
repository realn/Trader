#pragma once

#include <glm/fwd.hpp>
#include <CBSDL/Fwd.h>

#include "CoreEvents.h"

namespace core {
  class IInputMouseEvents {
  public:
    IInputMouseEvents() = default;
    IInputMouseEvents(IInputMouseEvents const&) = delete;
    virtual ~IInputMouseEvents() = default;

    virtual void OnMouseMotion(glm::vec2 const& pos, glm::vec2 const& delta) = 0;
    virtual void OnMouseButton(cb::sdl::Button const button, cb::sdl::KeyState const state) = 0;
  };

  class IInputKeyEvents {
  public:
    IInputKeyEvents() = default;
    IInputKeyEvents(IInputKeyEvents const&) = delete;
    virtual ~IInputKeyEvents() = default;

    virtual void OnKeyState(cb::sdl::ScanCode const code, cb::sdl::KeyState const state) = 0;
  };
}