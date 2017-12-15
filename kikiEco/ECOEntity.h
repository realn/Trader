#pragma once

#include <glm/vec2.hpp>

namespace eco {
  class CUniverse;
  class CEntity {
  private:
    std::weak_ptr<CUniverse> mUniverse;    
    cb::string mId;
    glm::vec2 mPosition;

  public:
    CEntity(cb::string const& id = cb::string()) : mId(id) {}

    void SetId(cb::string const& id) { mId = id; }
    void SetPosition(glm::vec2 const& value) { mPosition = value; }

    glm::vec2 GetPosition() const { return mPosition; }
    cb::string GetId() const { return mId; }
  };
}